#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <type_traits>

static_assert(std::is_same<decltype(&blockfill_s_16), blockfill_s_t>::value,
              "scalar must have the x265 primitive signature");
static_assert(std::is_same<decltype(&blockfill_s_16_rvv), blockfill_s_t>::value,
              "RVV must have the x265 primitive signature");

namespace {
constexpr size_t kCount = 8192;
constexpr intptr_t kOrigin = 4096;
uint64_t state = 0x12226554a38f196bULL;
size_t cases = 0;

uint64_t random_bits() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

void check(intptr_t stride, int16_t value) {
    std::array<int16_t, kCount> initial{}, scalar{}, vector{};
    std::array<uint8_t, kCount> touched{};
    for (size_t i = 0; i < kCount; ++i)
        initial[i] = static_cast<int16_t>(random_bits());
    scalar = vector = initial;

    // Oracle uses the address set, independently of both implementations.
    // Its bounding array also checks every untouched gap/guard element.
    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x) {
            intptr_t at = kOrigin + y * stride + x;
            if (at < 0 || at >= static_cast<intptr_t>(kCount))
                std::abort();
            touched[static_cast<size_t>(at)] = 1;
        }

    blockfill_s_16(scalar.data() + kOrigin, stride, value);
    blockfill_s_16_rvv(vector.data() + kOrigin, stride, value);
    for (size_t i = 0; i < kCount; ++i) {
        const int16_t expected = touched[i] ? value : initial[i];
        if (scalar[i] != expected || vector[i] != expected) {
            std::fprintf(stderr,
                         "case %zu stride=%ld val=%d index=%zu expected=%d "
                         "scalar=%d rvv=%d\n",
                         cases, static_cast<long>(stride),
                         static_cast<int>(value), i, static_cast<int>(expected),
                         static_cast<int>(scalar[i]),
                         static_cast<int>(vector[i]));
            std::exit(1);
        }
    }
    ++cases;
}
} // namespace

int main() {
    constexpr intptr_t strides[] = {-231, -128, -65, -31, -17, -16, -15, -8,
                                    -2,   -1,   0,   1,   2,   7,   8,   15,
                                    16,   17,   31,  64,  128, 231};
    constexpr int16_t values[] = {0, 1, -1, 32767, -32768, 0x5555, -21846};
    for (intptr_t stride : strides)
        for (int16_t value : values)
            for (int repeat = 0; repeat < 8; ++repeat)
                check(stride, value);
    for (int i = 0; i < 1024; ++i)
        check(static_cast<intptr_t>(random_bits() % 481) - 240,
              static_cast<int16_t>(random_bits()));
    std::printf("%zu exact full-buffer cases passed\n", cases);
}
