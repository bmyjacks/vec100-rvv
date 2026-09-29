#include "kernel.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <random>

constexpr uint32_t seed = 0x13326534;

struct State {
    std::array<uint16_t, 1024> scan{};
    std::array<int16_t, 2048> a{};
    std::array<int16_t, 2048> b{};
};

// alias=1: same source/destination; 2/3: offset source/destination;
// alias=4: scan/destination (both short's signed/unsigned views).
static uint32_t run(State &s, bool rvv, int count, int n, int alias) {
    int16_t *dst = s.a.data() + 32;
    const int16_t *resi = alias == 1 ? dst : alias == 2 ? dst + 1 :
                          alias == 3 ? dst - 1 : s.b.data() + 32;
    const uint16_t *scan = alias == 4 ? reinterpret_cast<uint16_t *>(dst) :
                           s.scan.data();
    return rvv ? rdo_recount_sign_rvv(scan, dst, resi, n, count) :
                 rdo_recount_sign(scan, dst, resi, n, count);
}

static int check(State s, int count, int n, int alias, unsigned &cases) {
    State v = s;
    const uint32_t expected = run(s, false, count, n, alias);
    const uint32_t actual = run(v, true, count, n, alias);
    if (expected != actual || s.scan != v.scan || s.a != v.a || s.b != v.b) {
        std::fprintf(stderr, "133 mismatch count=%d n=%d alias=%d case=%u seed=%u count=%u/%u\n",
                     count, n, alias, cases, seed, expected, actual);
        for (int i = 0; i < count; ++i)
            if (s.a[i + 32] != v.a[i + 32])
                std::fprintf(stderr, "  dst[%d]=%d/%d\n", i,
                             s.a[i + 32], v.a[i + 32]);
        return 1;
    }
    ++cases;
    return 0;
}

int main() {
    {
        State s;
        s.scan[0] = s.scan[1] = 3;
        s.a[35] = 7;
        s.b[35] = -1;
        // Same destination: 7 -> -7 -> 7; both reads are nonzero.
        if (run(s, true, 16, 2, 0) != 2 || s.a[35] != 7)
            return 2;
    }
    {
        State s;
        s.scan[0] = 2;
        s.scan[1] = 3;
        s.a[33] = -1; // residual for index 2
        s.a[34] = -8; // level at index 2; residual for index 3
        s.a[35] = -9;
        // First store changes the second residual sign (resi = dst - 1).
        if (run(s, true, 16, 2, 3) != 2 || s.a[34] != 8 || s.a[35] != -9)
            return 3;
    }
    {
        State s;
        s.a[32] = 1; // scan[0] = 1
        s.a[33] = 0; // scan[1] = 0, first store keeps it zero
        s.b[33] = -1;
        if (run(s, true, 16, 2, 4) != 1 || s.a[32] != 1)
            return 4;
    }
    {
        State s;
        s.a[32] = 1;  // scan[0] = 1
        s.a[33] = -2; // initially scan[1] is invalid; first store makes it 2
        s.a[34] = 5;
        s.b[33] = -1;
        s.b[34] = -1;
        // Each scan entry is valid when reached; pre-reading scan[1] is wrong.
        if (run(s, true, 16, 2, 4) != 2 || s.a[33] != 2 || s.a[34] != -5)
            return 5;
    }
    std::mt19937 rng(seed);
    unsigned cases = 0;
    const int lengths[] = {0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33,
                           63, 64, 65, 127, 255, 256, 257, 511, 1023, 1024};
    for (int count : {16, 64, 256, 1024}) {
        for (int n : lengths) {
            if (n > count) continue;
            for (int kind = 0; kind < 4; ++kind) {
                for (int alias = 0; alias < 5; ++alias) {
                    if (alias == 4 && n > count / 2) continue;
                    State s;
                    for (int i = 0; i < 2048; ++i) {
                        const int16_t values[] = {0, 1, 32767, -32768, -1,
                            static_cast<int16_t>(rng() % 65536)};
                        s.a[i] = values[rng() % 6];
                        s.b[i] = values[rng() % 6];
                    }
                    for (int i = 0; i < count; ++i)
                        s.scan[i] = uint16_t(i);
                    std::shuffle(s.scan.begin(), s.scan.begin() + count, rng);
                    if (kind == 1)
                        for (int i = 0; i < n; ++i) s.scan[i] = uint16_t(rng() % count);
                    if (kind == 2)
                        for (int i = 0; i < n; ++i) s.scan[i] = uint16_t(count - 1);
                    if (kind == 3)
                        for (int i = 0; i < n; ++i) s.scan[i] = uint16_t(i % 2);
                    if (alias == 4) {
                        // Stores hit dst >= count/2, outside the scan prefix.
                        // Keep actual scan entries valid even when aliasing.
                        for (int i = 0; i < n; ++i)
                            s.a[i + 32] = int16_t(count / 2 + rng() % (count / 2));
                    }
                    if (check(s, count, n, alias, cases)) return 1;
                }
            }
        }
    }
    std::printf("133: %u exact full-buffer cases (seed 0x%x)\n", cases, seed);
}
