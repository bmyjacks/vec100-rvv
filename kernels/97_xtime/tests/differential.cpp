#include "kernel.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <random>

SoftAesBlock softaes_inv_mix_columns_rvv(SoftAesBlock);
void softaes_invert_key_schedule128_rvv(SoftAesBlock[11]);
void softaes_invert_key_schedule256_rvv(SoftAesBlock[15]);

static SoftAesBlock random_block(std::mt19937 &rng) {
    return {static_cast<uint32_t>(rng()), static_cast<uint32_t>(rng()),
            static_cast<uint32_t>(rng()), static_cast<uint32_t>(rng())};
}

int main() {
    constexpr unsigned seed = 0x951a5u;
    std::mt19937 rng(seed);
    for (unsigned trial = 0; trial < 1000; ++trial) {
        const SoftAesBlock input = trial == 0   ? SoftAesBlock{}
                                   : trial == 1 ? SoftAesBlock{1, 0, 0, 0}
                                                : random_block(rng);
        const SoftAesBlock a = softaes_inv_mix_columns(input);
        const SoftAesBlock b = softaes_inv_mix_columns_rvv(input);
        if (std::memcmp(&a, &b, sizeof(a)) != 0) {
            std::fprintf(stderr,
                         "block trial=%u seed=%x in=%08x:%08x:%08x:%08x "
                         "scalar=%08x:%08x:%08x:%08x rvv=%08x:%08x:%08x:%08x\n",
                         trial, seed, input.w0, input.w1, input.w2, input.w3,
                         a.w0, a.w1, a.w2, a.w3, b.w0, b.w1, b.w2, b.w3);
            return 1;
        }
    }
    for (unsigned trial = 0; trial < 200; ++trial) {
        std::array<SoftAesBlock, 11> a128{}, b128{};
        std::array<SoftAesBlock, 15> a256{}, b256{};
        for (auto &block : a128)
            block = random_block(rng);
        for (auto &block : a256)
            block = random_block(rng);
        b128 = a128;
        b256 = a256;
        softaes_invert_key_schedule128(a128.data());
        softaes_invert_key_schedule128_rvv(b128.data());
        softaes_invert_key_schedule256(a256.data());
        softaes_invert_key_schedule256_rvv(b256.data());
        if (std::memcmp(a128.data(), b128.data(), sizeof(a128)) != 0 ||
            std::memcmp(a256.data(), b256.data(), sizeof(a256)) != 0) {
            std::fprintf(stderr, "schedule trial=%u seed=%x\n", trial, seed);
            return 1;
        }
    }
    std::printf("softaes invert: 1000 blocks and 400 schedules (seed %x)\n",
                seed);
}
