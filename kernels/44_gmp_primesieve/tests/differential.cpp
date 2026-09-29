#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

mp_limb_t gmp_primesieve_rvv(mp_ptr, mp_limb_t);

static constexpr unsigned seed = 0x44a35e1e;
static unsigned cases;

static void check(mp_limb_t n, std::mt19937_64 &rng) {
    const mp_limb_t last = ((n - 5) | mp_limb_t(1)) / 3;
    const size_t limbs = last / 64 + 1;
    std::vector<mp_limb_t> scalar(limbs + 2), vector(limbs + 2);
    for (size_t i = 0; i < scalar.size(); ++i)
        scalar[i] = vector[i] = rng();
    const mp_limb_t expected = gmp_primesieve(scalar.data() + 1, n);
    const mp_limb_t actual = gmp_primesieve_rvv(vector.data() + 1, n);
    if (expected != actual || scalar != vector) {
        for (size_t i = 0; i < scalar.size(); ++i)
            if (scalar[i] != vector[i]) {
                std::fprintf(stderr, "seed=%x case=%u n=%lu limb=%zu scalar=%lx rvv=%lx count=%lu/%lu\n",
                             seed, cases, n, i, scalar[i], vector[i], expected, actual);
                std::abort();
            }
        std::fprintf(stderr, "seed=%x case=%u n=%lu count=%lu/%lu\n",
                     seed, cases, n, expected, actual);
        std::abort();
    }

    // Independent prime oracle checks both the bitmap (including padded
    // high bits) and the returned count, not just agreement with the scalar.
    std::vector<unsigned char> prime(n + 1, 1);
    prime[0] = prime[1] = 0;
    for (mp_limb_t p = 2; p <= n / p; ++p)
        if (prime[p])
            for (mp_limb_t m = p * p; m <= n; m += p)
                prime[m] = 0;
    mp_limb_t count = 0;
    for (mp_limb_t bit = 0; bit < limbs * 64; ++bit) {
        const mp_limb_t number = 3 * bit + ((bit & 1) ? 4 : 5);
        const bool is_prime = number <= n && prime[number];
        count += is_prime;
        if (((vector[bit / 64 + 1] >> (bit % 64)) & 1) != !is_prime) {
            std::fprintf(stderr, "seed=%x case=%u n=%lu bit=%lu number=%lu\n",
                         seed, cases, n, bit, number);
            std::abort();
        }
    }
    if (actual != count) {
        std::fprintf(stderr, "seed=%x case=%u n=%lu oracle=%lu count=%lu\n",
                     seed, cases, n, count, actual);
        std::abort();
    }
    ++cases;
}

int main() {
    std::mt19937_64 rng(seed);
    for (mp_limb_t n = 5; n <= 600; ++n)
        check(n, rng);
    // The first primes past the pattern-fill seeds (11 and 13) enter the
    // marking scan at their squares. Exercise both sides of each transition.
    for (mp_limb_t p : {17UL, 19UL, 23UL, 29UL, 31UL, 37UL, 41UL,
                        43UL, 47UL, 53UL, 71UL, 73UL})
        for (mp_limb_t n = p * p - 3; n <= p * p + 3; ++n)
            check(n, rng);
    for (mp_limb_t limbs : {28UL, 29UL, 31UL, 32UL, 2048UL,
                            2049UL, 4095UL, 4096UL, 4097UL,
                            4107UL, 6144UL, 8193UL}) {
        mp_limb_t edge = 3 * 64 * limbs;
        for (mp_limb_t n = edge - 7; n <= edge + 7; ++n)
            check(n, rng);
    }
    for (unsigned i = 0; i < 30; ++i)
        check(5 + rng() % 1800000, rng);
    std::printf("gmp primesieve: %u exact bitmap/count/oracle cases (seed %x)\n",
                cases, seed);
}
