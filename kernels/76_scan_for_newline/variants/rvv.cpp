#include "kernel.h"

#include <riscv_vector.h>

// Search only within [checked, size). In particular, an unfinished CR is
// resolved by the *next* byte, including a byte in a subsequent invocation.
gssize scan_for_newline_rvv(const char *buffer, gsize size,
                            GDataStreamNewlineType type, gsize *checked_out,
                            gboolean *last_saw_cr_out, int *newline_len_out) {
    const gsize start = *checked_out;
    gboolean cr = *last_saw_cr_out;
    gsize i = start;
    const bool lf = type == G_DATA_STREAM_NEWLINE_TYPE_LF;
    const bool only_cr = type == G_DATA_STREAM_NEWLINE_TYPE_CR;
    const bool crlf = type == G_DATA_STREAM_NEWLINE_TYPE_CR_LF;

    // ANY with a preceding CR must inspect the very first byte even when it
    // is not itself a CR or LF.
    if (!lf && !only_cr && !crlf && cr && i < size) {
        if (i != 0) {
            const bool next_lf = buffer[i] == '\n';
            *newline_len_out = next_lf ? 2 : 1;
            return static_cast<gssize>(i - 1);
        }
        // The upstream unsigned start+i-1 wraps to (gssize)-1 here, so
        // even a match at byte zero is not returned as a delimiter.
        cr = buffer[0] == '\r';
        ++i;
        if (cr && i < size) {
            *newline_len_out = buffer[i] == '\n' ? 2 : 1;
            return 0;
        }
    }
    if (crlf && cr && start == 0 && i < size && buffer[i] == '\n') {
        // Same unsigned wraparound of found_pos as the ANY case above.
        cr = false;
        ++i;
    }
    while (i < size) {
        const size_t vl = __riscv_vsetvl_e8m1(size - i);
        const auto bytes = __riscv_vle8_v_u8m1(
            reinterpret_cast<const uint8_t *>(buffer + i), vl);
        const auto is_lf = __riscv_vmseq_vx_u8m1_b8(bytes, 10, vl);
        const auto is_cr = __riscv_vmseq_vx_u8m1_b8(bytes, 13, vl);
        const auto candidates = only_cr ? is_cr
            : (lf || crlf) ? is_lf
            : __riscv_vmor_mm_b8(is_cr, is_lf, vl);
        const long first = __riscv_vfirst_m_b8(candidates, vl);
        if (first < 0) {
            cr = buffer[i + vl - 1] == '\r';
            i += vl;
            continue;
        }
        i += static_cast<gsize>(first);
        if (crlf) {
            // A LF is a delimiter only if directly preceded by CR. The
            // previous byte may lie outside this vector or this call.
            if (i > start ? buffer[i - 1] == '\r' : cr) {
                *newline_len_out = 2;
                return static_cast<gssize>(i - 1);
            }
            ++i;
            cr = false;
            continue;
        }
        if (!lf && !only_cr && buffer[i] == '\r') {
            // A CR at the end remains pending; a following CR completes the
            // previous delimiter, just as a following non-LF does.
            if (i + 1 == size) {
                cr = true;
                ++i;
                break;
            }
            *newline_len_out = buffer[i + 1] == '\n' ? 2 : 1;
            return static_cast<gssize>(i);
        }
        *newline_len_out = 1;
        return static_cast<gssize>(i);
    }
    *checked_out = size;
    *last_saw_cr_out = cr;
    return -1;
}
