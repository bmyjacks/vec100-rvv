#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

void chacha20_encrypt_bytes_rvv(chacha_ctx *, const uint8_t *, uint8_t *,
                                unsigned long long);

int main() {
    std::mt19937 rng(0x6c4ac4a);
    const int lengths[] = {0, 1, 3, 15, 63, 64, 65, 127, 128, 129,
                           255, 256, 257, 1024, 1025, 4097};
    for (int length : lengths)
        for (int trial = 0; trial < 40; ++trial) {
            chacha_ctx a{}, b{};
            for (auto &word : a.input)
                word = rng();
            if (trial % 5 == 0)
                a.input[12] = 0xffffffffU - (trial % 3);
            b = a;
            std::vector<uint8_t> src(length + 128), out1(length + 128), out2(length + 128);
            for (auto &byte : src)
                byte = static_cast<uint8_t>(rng());
            out1 = src;
            out2 = src;
            const int offset = trial % 4;
            const int mode = trial % 3;
            const uint8_t *m1 = src.data() + offset;
            const uint8_t *m2 = src.data() + offset;
            uint8_t *c1 = out1.data() + offset + 8;
            uint8_t *c2 = out2.data() + offset + 8;
            if (mode == 1) { // exact in-place
                m1 = out1.data() + offset + 8;
                m2 = out2.data() + offset + 8;
            } else if (mode == 2) { // overlapping with a one-byte shift
                m1 = out1.data() + offset + 7;
                m2 = out2.data() + offset + 7;
            }
            chacha20_encrypt_bytes_isolated(&a, m1, c1, length);
            chacha20_encrypt_bytes_rvv(&b, m2, c2, length);
            if (std::memcmp(a.input, b.input, sizeof a.input) || out1 != out2) {
                std::fprintf(stderr, "chacha length=%d trial=%d mode=%d\n",
                             length, trial, mode);
                return 1;
            }
        }
    for (int length : {1, 8, 16, 64}) {
        chacha_ctx a{}, b{};
        for (auto &word : a.input)
            word = rng();
        b = a;
        uint8_t msg[64];
        for (auto &v : msg)
            v = static_cast<uint8_t>(rng());
        chacha20_encrypt_bytes_isolated(&a, msg,
            reinterpret_cast<uint8_t *>(&a), length);
        chacha20_encrypt_bytes_rvv(&b, msg,
            reinterpret_cast<uint8_t *>(&b), length);
        if (std::memcmp(&a, &b, sizeof a)) {
            std::fprintf(stderr, "chacha context alias length=%d\n", length);
            return 1;
        }
    }
    std::puts("chacha differential OK");
}
