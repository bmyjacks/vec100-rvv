#include "kernel.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

void mpn_fft_fft_rvv(mp_ptr *, mp_size_t, int **, mp_size_t, mp_size_t,
                        mp_size_t, mp_ptr);

namespace {
uint64_t state = 0xb170f17a3d593c8dULL;
uint64_t random_limb() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

void check(int id, int K, int n, int inc, int omega) {
    const mp_limb_t guard = 0xbadc0ffee0ddf00dUL;
    std::vector<std::vector<mp_limb_t>> ref(K), got(K);
    std::vector<mp_ptr> ap_ref(K * inc), ap_got(K * inc);
    std::vector<int> order(K);
    for (int i = 0; i < K; ++i) order[i] = i;
    // Reorder allocations so some indexed offsets wrap modulo 2^64.
    for (int i = K - 1; i > 0; --i)
        std::swap(order[i], order[random_limb() % (i + 1)]);
    for (int i = 0; i < K; ++i) {
        ref[i].assign(n + 3, guard);
        for (int h = 0; h < n; ++h) {
            const uint64_t r = random_limb();
            ref[i][h + 1] = (id % 11 == 0) ? ~mp_limb_t(0) :
                            (id % 13 == 0) ? 0 :
                            (id % 17 == 0) ? (r & 1) : r;
        }
        // Exercise both top-limb states, including the boundary 2^(64*n).
        ref[i][n + 1] = ((id + i) % 19 == 0 || id % 23 == 0);
        if (ref[i][n + 1] && id % 23 != 0)
            for (int h = 1; h <= n; ++h) ref[i][h] = 0;
        got[i] = ref[i];
    }
    for (int i = 0; i < K; ++i) {
        ap_ref[i * inc] = ref[order[i]].data() + 1;
        ap_got[i * inc] = got[order[i]].data() + 1;
    }

    int depth = 0;
    for (int t = K; t > 2; t >>= 1) ++depth;
    std::vector<std::vector<int>> levels(depth + 1);
    std::vector<int *> ll(depth + 1);
    for (int level = 1, size = 4; level <= depth; ++level, size <<= 1) {
        levels[level].resize(size);
        const int multiplier = omega << (depth - level);
        const int limit = (2 * n * 64 - 1) / multiplier;
        for (int j = 0; j < size / 2; ++j) {
            // Include limb/shift boundaries and both sides of the modulus.
            const int boundary[] = {0, 1, 63, 64, 65, 64 * n - 1,
                                    64 * n, 64 * n + 1, 2 * n * 64 - 1};
            int v = boundary[(id + j + level) % 9] / multiplier;
            if (id % 3 == 0) v = random_limb() % (limit + 1);
            levels[level][2 * j] = std::min(v, limit);
            levels[level][2 * j + 1] = 0;
        }
        ll[level] = levels[level].data();
    }
    std::vector<mp_limb_t> scratch_ref(2 * (n + 1) + 2, guard);
    std::vector<mp_limb_t> scratch_got = scratch_ref;
    mpn_fft_fft_isolated(ap_ref.data(), K, ll.data() + depth, omega, n, inc,
                       scratch_ref.data() + 1);
    mpn_fft_fft_rvv(ap_got.data(), K, ll.data() + depth, omega, n, inc,
                         scratch_got.data() + 1);
    for (int i = 0; i < K; ++i)
        if (ref[i] != got[i]) {
            std::fprintf(stderr, "mismatch seed=b170f17a3d593c8d case=%d K=%d n=%d inc=%d omega=%d residue=%d\n",
                         id, K, n, inc, omega, i);
            std::exit(1);
        }
    if (scratch_ref.front() != guard || scratch_ref.back() != guard ||
        scratch_got.front() != guard || scratch_got.back() != guard) {
        std::fprintf(stderr, "scratch guard case=%d\n", id);
        std::exit(1);
    }
}
} // namespace

int main() {
    for (int id = 0; id < 600; ++id) {
        const int K = 2 << (id % 6);
        const int n = 1 + ((id < 96) ? id % 4 : random_limb() % 48);
        const int inc = 1 + id % 3;
        const int omega = 1 + id % 3;
        check(id, K, n, inc, omega);
    }
    std::puts("600 seeded FFT differential cases passed");
}
