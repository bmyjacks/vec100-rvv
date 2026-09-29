#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

gssize scan_for_newline_rvv(const char *, gsize, GDataStreamNewlineType,
                            gsize *, gboolean *, int *);

static void check(const std::vector<char> &buf, gsize start,
                  GDataStreamNewlineType type, int saw_cr) {
    gsize a = start, b = start;
    gboolean ac = saw_cr, bc = saw_cr;
    int al = 99, bl = 99;
    const auto r1 = scan_for_newline_bench(buf.data(), buf.size(), type, &a, &ac, &al);
    const auto r2 = scan_for_newline_rvv(buf.data(), buf.size(), type, &b, &bc, &bl);
    if (r1 != r2 || a != b || ac != bc || al != bl) {
        std::fprintf(stderr, "mismatch n=%zu start=%zu type=%d cr=%d: "
                     "scalar=(%ld,%zu,%d,%d) rvv=(%ld,%zu,%d,%d)\n",
                     buf.size(), start, type, saw_cr, r1, a, ac, al, r2, b, bc, bl);
        std::abort();
    }
}

int main() {
    std::mt19937 rng(0x794321);
    const unsigned lengths[] = {0, 1, 2, 7, 15, 16, 17, 31, 32, 33,
                                63, 64, 65, 127, 128, 129, 255, 256, 257, 513};
    for (unsigned n : lengths) {
        for (int trial = 0; trial < 120; ++trial) {
            std::vector<char> buf(n + 1, 'q');
            for (unsigned i = 0; i < n; ++i) {
                unsigned v = rng() % 40;
                buf[i] = v == 0 ? '\r' : v == 1 ? '\n' : (char)('a' + v % 26);
            }
            buf.resize(n);
            for (auto start : {gsize(0), gsize(n / 2), gsize(n)})
                for (int type = 0; type <= 4; ++type)
                    for (int cr = 0; cr <= 1; ++cr)
                        check(buf, start, static_cast<GDataStreamNewlineType>(type), cr);
        }
    }
    // Split a CR/LF pair across calls, including at a vector boundary.
    for (int n : {16, 32, 64, 128}) {
        std::vector<char> buf(n + 1, 'q');
        buf[n - 1] = '\r'; buf[n] = '\n';
        for (int type : {2, 3}) {
            gsize a = 0, b = 0;
            gboolean ac = 0, bc = 0;
            int al = 88, bl = 88;
            auto t = static_cast<GDataStreamNewlineType>(type);
            auto r1 = scan_for_newline_bench(buf.data(), n, t, &a, &ac, &al);
            auto r2 = scan_for_newline_rvv(buf.data(), n, t, &b, &bc, &bl);
            if (r1 != r2 || a != b || ac != bc || al != bl) {
                std::fprintf(stderr, "split CR/LF first call n=%d type=%d scalar=(%ld,%zu,%d,%d) rvv=(%ld,%zu,%d,%d)\n",
                             n, type, r1, a, ac, al, r2, b, bc, bl);
                return 1;
            }
            r1 = scan_for_newline_bench(buf.data(), buf.size(), t, &a, &ac, &al);
            r2 = scan_for_newline_rvv(buf.data(), buf.size(), t, &b, &bc, &bl);
            if (r1 != r2 || a != b || ac != bc || al != bl) {
                std::fprintf(stderr, "split CR/LF second call n=%d type=%d scalar=(%ld,%zu,%d,%d) rvv=(%ld,%zu,%d,%d)\n",
                             n, type, r1, a, ac, al, r2, b, bc, bl);
                return 1;
            }
        }
    }
    std::puts("scan_for_newline: pass");
}
