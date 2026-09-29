#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

void copy_and_encode_rvv(lzma_delta_coder *, const uint8_t *, uint8_t *, size_t);
void encode_in_place_rvv(lzma_delta_coder *, uint8_t *, size_t);

int main() {
    std::mt19937 rng(0x8de17a);
    for (size_t distance = 0; distance <= 514; ++distance) {
        const size_t sizes[] = {0, 1, 2, 3, 4, 5, 6, 7, 15, 16, 17,
                                31, 32, 33, 63, 64, 65, 127, 128, 129,
                                255, 256, 257, 513};
        for (int trial = 0; trial < 28; ++trial) {
            const size_t n = trial < 24 ? sizes[trial] : (rng() % 900 + 256);
            lzma_delta_coder ref{}, vec{};
            ref.distance = distance;
            ref.pos = static_cast<uint8_t>(rng());
            for (auto &h : ref.history)
                h = static_cast<uint8_t>(rng());
            vec = ref;
            std::vector<uint8_t> input(n + 2), a(n + 2, 0xa5), b(n + 2, 0xa5);
            for (auto &v : input)
                v = static_cast<uint8_t>(rng());
            if (trial & 1) {
                a = input;
                b = input;
                encode_in_place_isolated(&ref, a.data() + 1, n);
                encode_in_place_rvv(&vec, b.data() + 1, n);
            } else {
                copy_and_encode_isolated(&ref, input.data() + 1, a.data() + 1, n);
                copy_and_encode_rvv(&vec, input.data() + 1, b.data() + 1, n);
            }
            if (a != b || ref.pos != vec.pos ||
                std::memcmp(ref.history, vec.history, sizeof ref.history)) {
                std::fprintf(stderr, "delta distance=%zu trial=%d n=%zu\n",
                             distance, trial, n);
                return 1;
            }
        }
    }
    for (int mode = 0; mode < 5; ++mode) {
        lzma_delta_coder a{}, b{};
        a.distance = 7;
        a.pos = 252;
        for (auto &v : a.history)
            v = static_cast<uint8_t>(rng());
        b = a;
        uint8_t input[100], out1[100], out2[100];
        for (auto &v : input)
            v = static_cast<uint8_t>(rng());
        std::memset(out1, 0xa5, sizeof out1);
        std::memset(out2, 0xa5, sizeof out2);
        if (mode == 0) {
            copy_and_encode_isolated(&a, a.history + 3, out1, 80);
            copy_and_encode_rvv(&b, b.history + 3, out2, 80);
        } else if (mode == 1) {
            copy_and_encode_isolated(&a, input, a.history + 5, 80);
            copy_and_encode_rvv(&b, input, b.history + 5, 80);
        } else if (mode == 2) {
            encode_in_place_isolated(&a, a.history + 5, 80);
            encode_in_place_rvv(&b, b.history + 5, 80);
        } else if (mode == 3) {
            encode_in_place_isolated(&a, &a.pos, 1);
            encode_in_place_rvv(&b, &b.pos, 1);
        } else {
            encode_in_place_isolated(&a, reinterpret_cast<uint8_t *>(&a.distance), 1);
            encode_in_place_rvv(&b, reinterpret_cast<uint8_t *>(&b.distance), 1);
        }
        if (std::memcmp(&a, &b, sizeof a) ||
            std::memcmp(out1, out2, 100)) {
            std::fprintf(stderr, "delta history alias mode=%d pos=%u/%u hist=%u/%u\n",
                         mode, a.pos, b.pos, a.history[252], b.history[252]);
            return 1;
        }
    }
    std::puts("delta differential OK");
}
