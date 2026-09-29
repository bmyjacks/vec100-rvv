#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

gchar *g_ascii_strdown(const gchar *, gssize);
gchar *g_ascii_strdown_rvv(const gchar *, gssize);

static unsigned long long seed = 0x29a5b6c7d8e9f123ULL;
static unsigned rnd() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return (unsigned)seed;
}

int main() {
    for (size_t n : {size_t(0), size_t(1), size_t(15), size_t(16),
                     size_t(17), size_t(31), size_t(32), size_t(127),
                     size_t(128), size_t(129), size_t(1025)}) {
        for (int trial = 0; trial < 60; ++trial) {
            std::vector<char> input(n + 1);
            for (size_t i = 0; i < n; ++i) {
                unsigned c = 1 + rnd() % 255;
                input[i] = (char)c;
            }
            input[n] = 0;
            if (n && trial % 4 == 0) input[rnd() % n] = 0;
            for (gssize len : {gssize(-3), gssize(-1), gssize(0), gssize(1),
                                (gssize)n / 2, (gssize)n, (gssize)n + 3}) {
                char *a = g_ascii_strdown(input.data(), len);
                char *b = g_ascii_strdown_rvv(input.data(), len);
                const size_t count = len < 0 ? std::strlen(input.data()) + 1 : (size_t)len + 1;
                if (!a || !b || std::memcmp(a, b, count)) {
                    std::fprintf(stderr, "strdown n=%zu len=%ld trial=%d\n", n, len, trial);
                    std::exit(1);
                }
                std::free(a);
                std::free(b);
            }
        }
    }
    std::puts("g_ascii_strdown: OK");
}
