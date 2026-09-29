#include "kernel.h"
#include <cstdio>
#include <vector>

int main() {
    uint32_t seed = 92;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    for (size_t size : {size_t(1), size_t(3), size_t(8), size_t(17)})
        for (int kind = 0; kind <= 3; ++kind)
            for (bool wide : {false, true}) {
        std::vector<GB_void> src(256 * size), ref(160 * size), out(160 * size);
        for (auto &x : src) x = rnd();
        for (auto &x : ref) x = rnd();
        out = ref;
        std::vector<uint32_t> i32(96);
        for (auto &x : i32) x = rnd() % 110;
        std::vector<uint64_t> i64(i32.begin(), i32.end());
        int64_t colon[] = {kind == GB_STRIDE && wide ? 210 : 7,
                           0, kind == GB_STRIDE ? (wide ? -2 : 2) : 1};
        auto args = [&](auto fn, GB_void *dst, const GB_void *input) {
            fn(dst, input, size, wide ? nullptr : i32.data(),
               wide ? i64.data() : nullptr, kind, colon, 3, 94, 5, 11);
        };
        args(GB_bitmap_subref, ref.data(), src.data());
        args(GB_bitmap_subref_rvv, out.data(), src.data());
        for (size_t i = 0; i < ref.size(); ++i) if (ref[i] != out[i]) {
            std::fprintf(stderr, "bitmap subref mismatch size=%zu kind=%d wide=%d index=%zu scalar=%u rvv=%u\n",
                         size, kind, wide, i, unsigned(ref[i]), unsigned(out[i]));
            return 1;
        }
    }
    // The same backing allocation is a matrix view in both directions.
    // Separate calls use identical initial storage and ordered scalar copies.
    std::vector<GB_void> ref(256), out(256);
    for (auto &x : ref) x = rnd();
    out = ref;
    uint32_t list[60];
    for (auto &x : list) x = 20 + rnd() % 45;
    // Overlap is outside the extracted restrict-qualified API; compare the
    // variant's ordered fallback with explicit one-byte copy semantics.
    for (int i = 0; i < 60; ++i) ref[24 + i] = ref[list[i]];
    GB_bitmap_subref_rvv(out.data(), out.data(), 1, list, nullptr, GB_LIST,
                         nullptr, 0, 60, 0, 24);
    for (size_t i = 0; i < ref.size(); ++i) if (ref[i] != out[i]) {
        std::fprintf(stderr, "bitmap subref overlap mismatch index=%zu expected=%u rvv=%u\n",
                     i, unsigned(ref[i]), unsigned(out[i]));
        return 1;
    }
}
