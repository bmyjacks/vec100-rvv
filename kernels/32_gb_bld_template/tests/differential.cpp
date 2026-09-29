#include "kernel.h"
#include <cstdio>
#include <cstring>
#include <vector>

int main() {
    uint32_t seed = 771;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    constexpr int n = 147;
    std::vector<double> sx(n), ref(n + 9), out(n + 9);
    std::vector<uint32_t> k32(n), iw32(n);
    for (int p = 0; p < n; ++p) {
        sx[p] = double(int(rnd() % 203) - 101) / 8;
        k32[p] = rnd() % n;
        iw32[p] = p % 7 == 3 ? UINT32_MAX : p + 7;
    }
    // Duplicates owned by the preceding task extend past both boundaries.
    iw32[41] = iw32[42] = iw32[98] = iw32[99] = UINT32_MAX;
    sx[40] = 1e20;
    sx[41] = -1e20;
    sx[42] = 1.0;
    std::vector<uint64_t> k64(k32.begin(), k32.end()), iw64(iw32.begin(), iw32.end());
    int64_t slices[] = {0, 41, 98, n}, tnz[] = {2, 45, 103};
    for (bool kwide : {false, true}) for (bool with_k : {false, true}) {
        for (auto &v : ref) v = int(rnd() % 17);
        out = ref;
        const void *perm = with_k ? (kwide ? static_cast<const void *>(k64.data())
                                           : static_cast<const void *>(k32.data())) : nullptr;
        GB_bld__plus_fp64(ref.data(), nullptr, false, sx.data(), n, 0,
                           nullptr, false, perm, !kwide, 0, slices, nullptr, 3);
        GB_bld__plus_fp64_rvv(out.data(), nullptr, false, sx.data(), n, 0,
                               nullptr, false, perm, !kwide, 0, slices, nullptr, 3);
        for (size_t i = 0; i < ref.size(); ++i)
            if (std::memcmp(&ref[i], &out[i], sizeof(double))) {
                std::fprintf(stderr, "bld plus mismatch kwide=%d with_k=%d index=%zu scalar=%a rvv=%a\n",
                             kwide, with_k, i, ref[i], out[i]);
                return 1;
            }
        if (with_k) {
            GB_bld_template(sx.data(), ref.data(), perm,
                            kwide ? nullptr : k32.data(), kwide ? k64.data() : nullptr,
                            slices, 3);
            GB_bld_template_rvv(sx.data(), out.data(), perm,
                                kwide ? nullptr : k32.data(), kwide ? k64.data() : nullptr,
                                slices, 3);
            if (ref != out) {
                std::fprintf(stderr, "bld template mismatch kwide=%d with_k=%d\n", kwide, with_k);
                return 1;
            }
        }
    }
    for (bool iwide : {false, true}) for (bool kwide : {false, true})
        for (bool with_k : {false, true}) for (bool tiwide : {false, true}) {
        std::vector<double> r(n + 9, -19), v = r;
        std::vector<uint32_t> ir(n + 9, 9999), iv = ir;
        std::vector<uint64_t> lr(n + 9, 9999), lv = lr;
        auto call = [&](auto fn, double *tx, void *ti) {
            return fn(tx, ti, tiwide, sx.data(), n, 20,
                      iwide ? static_cast<const void *>(iw32.data())
                            : static_cast<const void *>(iw64.data()), iwide,
                      with_k ? (kwide ? static_cast<const void *>(k32.data())
                                      : static_cast<const void *>(k64.data())) : nullptr,
                      kwide, UINT32_MAX, slices, tnz, 3);
        };
        const auto scalar_status = call(GB_bld__plus_fp64, r.data(), tiwide ?
                    static_cast<void *>(ir.data()) : static_cast<void *>(lr.data()));
        const auto rvv_status = call(GB_bld__plus_fp64_rvv, v.data(), tiwide ?
                    static_cast<void *>(iv.data()) : static_cast<void *>(lv.data()));
        if (scalar_status != rvv_status || scalar_status != GrB_SUCCESS ||
            r != v || ir != iv || lr != lv) {
            std::fprintf(stderr, "bld mismatch iwide=%d kwide=%d with_k=%d tiwide=%d status=%d/%d values=%d indices32=%d indices64=%d\n",
                         iwide, kwide, with_k, tiwide, int(scalar_status), int(rvv_status),
                         r != v, ir != iv, lr != lv);
            return 1;
        }
    }
}
