#include "kernel.h"
#include <cstdio>
#include <cstring>
#include <vector>

int main() {
    uint32_t seed = 0x1234abcd;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    for (bool wide : {false, true}) for (bool hyper : {false, true})
        for (bool aiso : {false, true}) for (bool biso : {false, true})
            for (bool duplicates : {false, true}) {
        const int64_t vlen = 73, cnz = vlen * 3;
        std::vector<uint32_t> bp32{0, 51, 104, 161}, bh32{2, 0, 1};
        std::vector<uint64_t> bp64(bp32.begin(), bp32.end());
        std::vector<uint64_t> bh64(bh32.begin(), bh32.end());
        std::vector<int32_t> bi32(161);
        for (int k = 0; k < 3; ++k)
            for (int p = bp32[k]; p < int(bp32[k + 1]); ++p)
                bi32[p] = duplicates ? (p - bp32[k]) / 2 : p - bp32[k];
        std::vector<int64_t> bi64(bi32.begin(), bi32.end());
        std::vector<double> ax(cnz), bx(161), ref(cnz + 11), out(cnz + 11);
        for (auto &a : ax) a = int(rnd() % 97) * .125;
        for (auto &b : bx) b = int(rnd() % 37) * .25;
        for (auto &c : ref) c = int(rnd() % 73);
        out = ref;
        // Two tasks split the first vector; third task handles the rest.
        // Duplicate destinations are exercised in the one-task case.
        std::vector<int64_t> slices = duplicates
            ? std::vector<int64_t>{0, 2, 0, 161}
            : std::vector<int64_t>{0, 0, 2, 0, 1, 2, 0, 27, 104, 161};
        int tasks = duplicates ? 1 : 3;
        auto args = [&](auto fn, double *dest) {
            fn(wide ? nullptr : bp32.data(), wide ? bp64.data() : nullptr,
               wide ? nullptr : bi32.data(), wide ? bi64.data() : nullptr,
               hyper && !wide ? bh32.data() : nullptr,
               hyper && wide ? bh64.data() : nullptr, bx.data(), ax.data(),
               dest, slices.data(), tasks, 1, vlen, aiso, biso, cnz, 1, 8.0);
        };
        args(GB_add_full_32, ref.data());
        args(GB_add_full_32_rvv, out.data());
        for (size_t i = 0; i < ref.size(); ++i)
            if (std::memcmp(&ref[i], &out[i], sizeof(double))) {
                std::fprintf(stderr, "add full mismatch wide=%d hyper=%d aiso=%d biso=%d duplicates=%d index=%zu scalar=%a rvv=%a\n",
                             wide, hyper, aiso, biso, duplicates, i, ref[i], out[i]);
                return 1;
            }
    }
}
