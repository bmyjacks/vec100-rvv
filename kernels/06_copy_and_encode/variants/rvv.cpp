#include "kernel.h"

#include <riscv_vector.h>

static bool touches_coder(const uint8_t *p, size_t n,
                          const lzma_delta_coder *coder) {
    if (!n)
        return false;
    const uintptr_t a = reinterpret_cast<uintptr_t>(p);
    const uintptr_t h = reinterpret_cast<uintptr_t>(coder);
    return (a >= h && a - h < sizeof(*coder)) || (h > a && h - a < n);
}

// Limit each group to distance samples. The history reads then all precede
// their corresponding writes, even when the 256-byte ring wraps.
static void encode(lzma_delta_coder *coder, const uint8_t *in, uint8_t *out,
                   size_t size) {
    const size_t distance = coder->distance;
    if (touches_coder(in, size, coder) ||
        touches_coder(out, size, coder)) {
        for (size_t i = 0; i < size; ++i) {
            const uint8_t previous = coder->history[(distance + coder->pos) & 255];
            coder->history[coder->pos-- & 255] = in[i];
            out[i] = static_cast<uint8_t>(in[i] - previous);
        }
        return;
    }
    const size_t gap = (distance & 255) ? distance & 255 : 256;
    size_t i = 0;
    while (i < size) {
        size_t available = size - i;
        if (available > gap)
            available = gap;
        const size_t vl = __riscv_vsetvl_e8m1(available);
        const auto indices = __riscv_vid_v_u8m1(vl);
        const auto write_indices = __riscv_vrsub_vx_u8m1(
            indices, coder->pos, vl);
        const auto read_indices = __riscv_vadd_vx_u8m1(
            write_indices, static_cast<uint8_t>(distance), vl);
        const auto previous = __riscv_vluxei8_v_u8m1(
            coder->history, read_indices, vl);
        const auto current = __riscv_vle8_v_u8m1(in + i, vl);
        const auto encoded = __riscv_vsub_vv_u8m1(current, previous, vl);
        __riscv_vsuxei8_v_u8m1(coder->history, write_indices, current, vl);
        __riscv_vse8_v_u8m1(out + i, encoded, vl);
        coder->pos = static_cast<uint8_t>(coder->pos - vl);
        i += vl;
    }
}

void copy_and_encode_rvv(lzma_delta_coder *coder, const uint8_t *in,
                         uint8_t *out, size_t size) {
    encode(coder, in, out, size);
}

void encode_in_place_rvv(lzma_delta_coder *coder, uint8_t *buffer,
                         size_t size) {
    encode(coder, buffer, buffer, size);
}
