#include "kernel.h"

#include <array>
#include <cstdio>
#include <limits>
#include <random>

void lz4_renorm_hash_table_rvv(LZ4_hash_table_view *, U32);

int main() {
    constexpr unsigned seed = 0x14a011u;
    std::mt19937 rng(seed);
    const std::array<U32, 7> deltas = {0u,
                                       1u,
                                       65535u,
                                       65536u,
                                       0x7fffffffu,
                                       0x80000000u,
                                       std::numeric_limits<U32>::max()};
    unsigned cases = 0;
    for (U32 delta : deltas) {
        for (int mode = 0; mode < 9; ++mode) {
            LZ4_hash_table_view a{}, b{};
            for (int i = 0; i < LZ4_HASH_SIZE_U32; ++i) {
                U32 v = rng();
                switch (mode) {
                case 0:
                    v = 0;
                    break;
                case 1:
                    v = std::numeric_limits<U32>::max();
                    break;
                case 2:
                    v = delta;
                    break;
                case 3:
                    v = delta == 0 ? 0 : delta - 1;
                    break;
                case 4:
                    v = delta == UINT32_MAX ? delta : delta + 1;
                    break;
                case 5:
                    v = i & 1 ? 0 : UINT32_MAX;
                    break;
                case 6:
                    v = static_cast<U32>(i);
                    break;
                default:
                    break;
                }
                a.hashTable[i] = b.hashTable[i] = v;
            }
            lz4_renorm_hash_table(&a, delta);
            lz4_renorm_hash_table_rvv(&b, delta);
            for (int i = 0; i < LZ4_HASH_SIZE_U32; ++i) {
                if (a.hashTable[i] != b.hashTable[i]) {
                    std::fprintf(
                        stderr,
                        "delta=%u mode=%d index=%d seed=%x scalar=%u rvv=%u\n",
                        delta, mode, i, seed, a.hashTable[i], b.hashTable[i]);
                    return 1;
                }
            }
            ++cases;
        }
    }
    std::printf("lz4 hash rescale: %u exact cases (seed %x)\n", cases, seed);
}
