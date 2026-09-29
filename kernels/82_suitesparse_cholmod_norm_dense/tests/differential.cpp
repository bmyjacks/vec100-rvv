#include "kernel.h"

#include <array>
#include <cfenv>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

using suitesparse::Int;

static double from_bits(uint64_t bits) {
    double value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint64_t bits(double value) {
    uint64_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static unsigned cases = 0;

static bool check(Int rows, Int cols, Int stride, const std::vector<double> &data) {
    const double *p = data.empty() ? nullptr : data.data();
    double scalar = suitesparse::cholmod_norm_dense(rows, cols, stride, p);
    double vector = suitesparse::cholmod_norm_dense_rvv(rows, cols, stride, p);
    if (bits(scalar) != bits(vector)) {
        std::fprintf(stderr, "case %u rows=%lld cols=%lld d=%lld: %016llx != %016llx\n",
                     cases, static_cast<long long>(rows), static_cast<long long>(cols),
                     static_cast<long long>(stride),
                     static_cast<unsigned long long>(bits(scalar)),
                     static_cast<unsigned long long>(bits(vector)));
        return false;
    }
    ++cases;
    return true;
}

int main() {
    constexpr uint64_t seed = 0x126c401d13ULL;
    std::mt19937_64 rng(seed);
    const std::array<double, 16> edges = {
        0., -0., 1., -1., 1e16, -1e16, 1e-300, -1e-300,
        std::numeric_limits<double>::min(),
        -std::numeric_limits<double>::denorm_min(),
        std::numeric_limits<double>::max(),
        -std::numeric_limits<double>::infinity(),
        from_bits(0x7ff8000000001234ULL), from_bits(0xfff8000000005678ULL),
        from_bits(0x7ff0000000001234ULL), from_bits(0xfff0000000005678ULL)};

    // Empty shapes, tails, zero and nonzero padding, multiple columns.
    for (Int rows : {Int(0), Int(1), Int(2), Int(3), Int(4), Int(7),
                     Int(31), Int(63), Int(64), Int(65), Int(127),
                     Int(128), Int(129), Int(257)}) {
        for (Int cols : {Int(0), Int(1), Int(2), Int(5), Int(9)}) {
            for (Int pad : {Int(0), Int(1), Int(5)}) {
                const Int d = rows + pad;
                std::vector<double> data(static_cast<size_t>(d * cols),
                                         from_bits(0x7ff800000000deadULL));
                for (Int j = 0; j < cols; ++j) {
                    for (Int i = 0; i < rows; ++i) {
                        data[static_cast<size_t>(i + j * d)] =
                            edges[static_cast<size_t>(i + 3 * j) % edges.size()];
                    }
                }
                if (!check(rows, cols, d, data)) return 1;
                for (Int j = 0; j < cols; ++j) {
                    for (Int i = 0; i < rows; ++i) {
                        data[static_cast<size_t>(i + j * d)] =
                            from_bits(rng());
                    }
                }
                if (!check(rows, cols, d, data)) return 1;
            }
        }
    }
    // Cases sensitive to sequential rounding, NaN propagation and layout.
    const std::vector<std::vector<double>> special = {
             {1e16, 1., 1., 1., 1e16},
             {from_bits(0xfff8000000001234ULL), 1.,
              from_bits(0x7ff8000000005678ULL)},
             {1., -0., std::numeric_limits<double>::infinity()},
             {3., 4., 0., -100., 0., 2.}};
    for (const auto &values : special) {
        if (!check(static_cast<Int>(values.size()), 1,
                   static_cast<Int>(values.size()), values)) return 1;
    }
    const std::vector<double> layout = {-1., 2., -999., -999.,
                                         -4., -5., -999., -999.};
    if (!check(2, 2, 4, layout) ||
        bits(suitesparse::cholmod_norm_dense_rvv(2, 2, 4, layout.data())) !=
            bits(9.)) return 1;
    const std::vector<double> rounded = {1e16, 1., 1., 1.};
    for (int mode : {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO}) {
        if (std::fesetround(mode) || !check(4, 1, 4, rounded)) return 1;
    }
    if (std::fesetround(FE_TONEAREST)) return 2;
    for (unsigned t = 0; t < 500; ++t) {
        Int rows = static_cast<Int>(rng() % 301);
        Int cols = static_cast<Int>(rng() % 13);
        Int d = rows + static_cast<Int>(rng() % 8);
        std::vector<double> data(static_cast<size_t>(d * cols),
                                 from_bits(0x7ff800000000beefULL));
        for (Int j = 0; j < cols; ++j) {
            for (Int i = 0; i < rows; ++i) {
                data[static_cast<size_t>(i + j * d)] =
                    t % 3 == 0 ? edges[rng() % edges.size()]
                               : from_bits(rng());
            }
        }
        if (!check(rows, cols, d, data)) return 1;
    }
    std::printf("cholmod norm dense: %u bit-exact cases (seed %llx)\n",
                cases, static_cast<unsigned long long>(seed));
}
