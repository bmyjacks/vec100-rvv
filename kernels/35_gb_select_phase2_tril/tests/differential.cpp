#include "kernel.h"
#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    uint32_t seed = 36;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    // The standalone matrix wrapper sets vdim=1: split that one column.
    const uint64_t ap[] = {0, 210};
    const int64_t slice[] = {0, 0, 0, 0, 0, 31, 210};
    std::vector<int64_t> ai(210);
    std::vector<double> ax(210);
    for (int p = 0; p < 210; ++p) {
        ai[p] = p + 2;
        ax[p] = int(rnd() % 41) * .25;
    }
    for (bool triu : {false, true}) for (uint64_t pivot : {0u, 20u, 65u, 210u, 260u}) {
        uint64_t zp[] = {pivot};
        auto kept = [&](uint64_t start, uint64_t end, uint64_t z) {
            return triu ? (z > start ? std::min(end, z) - start : 0u)
                        : (z < end ? end - std::max(start, z) : 0u);
        };
        uint64_t cp[] = {0, kept(0, 210, pivot)};
        uint64_t cfirst[] = {0, kept(0, 31, zp[0])};
        std::vector<int64_t> ir(400), iv(400);
        std::vector<double> xr(400), xv(400);
        for (auto &x : ir) x = rnd();
        iv = ir;
        for (auto &x : xr) x = rnd() % 23;
        xv = xr;
        const auto scalar_status = GB_select_phase2_tril(ap, ai.data(), ax.data(), cp,
                   ir.data(), xr.data(), zp, slice, cfirst, 256, triu, 2, 1);
        const auto rvv_status = GB_select_phase2_tril_rvv(ap, ai.data(), ax.data(), cp,
                   iv.data(), xv.data(), zp, slice, cfirst, 256, triu, 2, 1);
        if (scalar_status != rvv_status || scalar_status != GrB_SUCCESS) {
            std::fprintf(stderr, "tril triu=%d pivot=%lu scalar status=%d rvv status=%d\n",
                         triu, pivot, int(scalar_status), int(rvv_status));
            return 1;
        }
        for (size_t p = 0; p < ir.size(); ++p)
            if (ir[p] != iv[p] || xr[p] != xv[p]) {
                std::fprintf(stderr, "tril triu=%d pivot=%lu p=%zu i=%ld/%ld x=%g/%g\n",
                             triu, pivot, p, ir[p], iv[p], xr[p], xv[p]);
                return 1;
            }
    }
}
