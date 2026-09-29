#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

void utf8_verify_ascii_rvv(const char **, gsize *);

static unsigned long long state = 0xb074312d9ac6e15fULL;
static unsigned random_byte() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return static_cast<unsigned>(state & 255);
}

static void check(const char *data, gsize len, bool c_string,
                  gsize expected_limit, unsigned shift) {
    const char *scalar_pos = data;
    const char *vector_pos = data;
    gsize scalar_len = len;
    gsize vector_len = len;
    utf8_verify_ascii_isolated(&scalar_pos, c_string ? nullptr : &scalar_len);
    utf8_verify_ascii_rvv(&vector_pos, c_string ? nullptr : &vector_len);
    if (vector_pos - data != scalar_pos - data ||
        (!c_string && vector_len != scalar_len) ||
        static_cast<gsize>(vector_pos - data) > expected_limit) {
        std::fprintf(stderr, "ascii len=%zu c_string=%d shift=%u: offset %td/%td remaining %zu/%zu\n",
                     len, c_string, shift, vector_pos - data,
                     scalar_pos - data, vector_len, scalar_len);
        std::exit(1);
    }
}

int main() {
    const gsize sizes[] = {0, 1, 2, 7, 8, 15, 16, 17, 31, 32, 33,
                           63, 64, 65, 127, 128, 129, 256, 1025};
    for (gsize len : sizes) {
        for (unsigned shift = 0; shift < 16; ++shift) {
            std::vector<char> buf(len + shift + 2, 'a');
            char *data = buf.data() + shift;
            data[len] = 0;
            // Entirely ASCII, with and without an explicit length.
            check(data, len, false, len, shift);
            check(data, len, true, len, shift);
            for (gsize pos : {gsize(0), gsize(1), gsize(7), gsize(8),
                              gsize(15), gsize(16), gsize(17), gsize(31),
                              gsize(32), gsize(127), len / 2, len - (len != 0)}) {
                if (pos >= len) continue;
                for (unsigned bad : {0u, 0x80u, 0xffu}) {
                    data[pos] = static_cast<char>(bad);
                    check(data, len, false, len, shift);
                    // strlen's length must end at the injected NUL.
                    check(data, len, true, len, shift);
                    data[pos] = 'a';
                }
            }
            for (unsigned round = 0; round < 16; ++round) {
                for (gsize i = 0; i < len; ++i)
                    data[i] = static_cast<char>(random_byte());
                data[len] = 0;
                check(data, len, false, len, shift);
                check(data, len, true, len, shift);
            }
        }
    }
    // The explicit-length interface must never fetch bytes from a guard page.
    const gsize page = static_cast<gsize>(sysconf(_SC_PAGESIZE));
    void *region = mmap(nullptr, 2 * page, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED || mprotect(static_cast<char *>(region) + page,
                                        page, PROT_NONE)) std::abort();
    for (gsize len : {gsize(0), gsize(1), gsize(7), gsize(8),
                      gsize(16), gsize(17), gsize(127), gsize(256)}) {
        char *data = static_cast<char *>(region) + page - len;
        memset(data, 'x', len);
        check(data, len, false, len, 0);
        if (len) {
            data[len - 1] = static_cast<char>(0xff);
            check(data, len, false, len, 0);
            data[len - 1] = 0;
            check(data, len, false, len, 0);
            check(data, len, true, len, 0);
        }
    }
    munmap(region, 2 * page);
    std::puts("utf8_verify_ascii: OK");
}
