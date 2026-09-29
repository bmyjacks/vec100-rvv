#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

int caxpy_k_rvv(BLASLONG, BLASLONG, BLASLONG, float, float, float *, BLASLONG,
                float *, BLASLONG, float *, BLASLONG);

int main() {
    std::mt19937 rng(0x5ca012);
    const BLASLONG strides[] = {-3, -1, 0, 1, 2, 4};
    for (BLASLONG n : {-3L, 0L, 1L, 2L, 3L, 4L, 7L, 15L, 17L, 64L, 257L}) {
        for (BLASLONG sx : strides)
            for (BLASLONG sy : strides)
                for (int overlap = 0; overlap < 3; ++overlap) {
                    std::vector<float> ref(12000), vec(12000);
                    for (float &v : ref)
                        v = static_cast<int>(rng() % 4096) / 1024.0f - 2.0f;
                    vec = ref;
                    const int xbase = 3100, ybase = overlap == 0 ? 8500 : xbase + overlap - 1;
                    auto x = [&](std::vector<float> &a) { return a.data() + xbase; };
                    auto y = [&](std::vector<float> &a) { return a.data() + ybase; };
                    const float ar = (rng() % 7 - 3.0f) / 7.0f;
                    const float ai = (rng() % 9 - 4.0f) / 9.0f;
                    const int r = caxpy_k(n, 3, 4, ar, ai, x(ref), sx, y(ref), sy, nullptr, 7);
                    const int v = caxpy_k_rvv(n, 3, 4, ar, ai, x(vec), sx, y(vec), sy, nullptr, 7);
                    if (r != v || std::memcmp(ref.data(), vec.data(), ref.size() * sizeof(float))) {
                        std::fprintf(stderr, "caxpy n=%ld sx=%ld sy=%ld overlap=%d\n", n, sx, sy, overlap);
                        for (size_t k = 0; k < ref.size(); ++k)
                            if (std::memcmp(&ref[k], &vec[k], sizeof(float))) {
                                std::fprintf(stderr, "index=%zu ref=%a vec=%a\n", k,
                                             ref[k], vec[k]);
                                break;
                            }
                        return 1;
                    }
                }
    }
    std::puts("caxpy differential OK");
}
