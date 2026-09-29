#include "kernel.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

int sscal_k_rvv(BLASLONG, BLASLONG, BLASLONG, FLOAT, FLOAT *, BLASLONG,
                FLOAT *, BLASLONG, FLOAT *, BLASLONG);

static uint32_t bits(float value) {
    uint32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static bool check(BLASLONG n, BLASLONG stride, float factor, BLASLONG branch,
                  const std::vector<float> &input) {
    std::vector<float> scalar = input;
    std::vector<float> vector = input;
    const int a = sscal_k(n, 17, 19, factor, scalar.data(), stride,
                          nullptr, 0, nullptr, branch);
    const int b = sscal_k_rvv(n, 17, 19, factor, vector.data(), stride, nullptr, 0,
                              nullptr, branch);
    if (a != b)
        return false;
    for (size_t i = 0; i < input.size(); ++i) {
        if (bits(scalar[i]) != bits(vector[i])) {
            std::fprintf(stderr,
                         "n=%ld stride=%ld branch=%ld factor=%08x index=%zu "
                         "input=%08x scalar=%08x rvv=%08x\n",
                         n, stride, branch, bits(factor), i, bits(input[i]),
                         bits(scalar[i]), bits(vector[i]));
            return false;
        }
    }
    return true;
}

int main() {
    // Report the seed so a failure is reproducible under another VLEN.
    constexpr unsigned seed = 0x85ca1u;
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> value(-1000.0f, 1000.0f);
    const std::array<float, 9> factors = {
        0.0f,
        -0.0f,
        1.0f,
        -1.0f,
        0.25f,
        -3.5f,
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::quiet_NaN()};
    const std::array<float, 9> specials = {
        0.0f,
        -0.0f,
        1.0f,
        -1.0f,
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::denorm_min(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::quiet_NaN()};

    unsigned cases = 0;
    for (BLASLONG n : {-3L, 0L, 1L, 2L, 3L, 4L, 5L, 7L, 8L, 15L, 16L, 17L, 31L,
                       32L, 33L, 63L, 64L, 65L, 129L}) {
        for (BLASLONG stride : {-1L, 0L, 1L, 2L, 3L, 7L}) {
            for (BLASLONG branch : {0L, 1L, 2L}) {
                for (float factor : factors) {
                    const size_t count =
                        n > 0 && stride > 0
                            ? static_cast<size_t>((n - 1) * stride + 4)
                            : 4;
                    std::vector<float> data(count);
                    for (size_t j = 0; j < count; ++j)
                        data[j] = j % 5 == 0
                                      ? specials[(j + cases) % specials.size()]
                                      : value(rng);
                    if (!check(n, stride, factor, branch, data))
                        return 1;
                    ++cases;
                }
            }
        }
    }
    for (unsigned trial = 0; trial < 300; ++trial) {
        const BLASLONG n = static_cast<BLASLONG>(rng() % 257);
        const BLASLONG stride = 1 + static_cast<BLASLONG>(rng() % 9);
        const float factor =
            trial % 7 == 0 ? factors[rng() % factors.size()] : value(rng);
        const BLASLONG branch = static_cast<BLASLONG>(rng() % 3);
        std::vector<float> data(static_cast<size_t>(n * stride + 4));
        for (float &element : data)
            element = value(rng);
        for (size_t i = 0; i < data.size(); i += 11)
            data[i] = specials[(i + trial) % specials.size()];
        if (!check(n, stride, factor, branch, data))
            return 1;
        ++cases;
    }
    std::printf("sscal_k: %u bit-exact differential cases (seed %x)\n", cases,
                seed);
}
