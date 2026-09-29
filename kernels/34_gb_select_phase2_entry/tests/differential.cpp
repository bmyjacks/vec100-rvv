#include "kernel.h"
#include <cstdio>
#include <limits>
#include <vector>

int main() {
    uint32_t seed = 35;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    // The standalone matrix wrapper sets vdim=1: split that one column.
    const uint64_t ap[] = {0, 210};
    const int64_t slice[] = {0, 0, 0, 0, 0, 31, 210};
    std::vector<int64_t> ai(210);
    std::vector<double> ax(210);
    for (int p = 0; p < 210; ++p) {
        ai[p] = p + 2;
        ax[p] = int(rnd() % 17) - 8;
    }
    ax[17] = std::numeric_limits<double>::quiet_NaN();
    for (double threshold : {-9.0, 0.0, 8.0,
                             std::numeric_limits<double>::quiet_NaN()}) {
        uint64_t cp[] = {0, 0};
        for (int p = 0; p < 210; ++p) cp[1] += ax[p] > threshold;
        uint64_t cfirst[] = {0, 0};
        for (int p = 0; p < 31; ++p) cfirst[1] += ax[p] > threshold;
        std::vector<int64_t> ir(400), iv(400);
        std::vector<double> xr(400), xv(400);
        for (auto &x : ir) x = rnd();
        iv = ir;
        for (auto &x : xr) x = rnd() % 23;
        xv = xr;
        const auto scalar_status = GB_select_phase2_entry(ap, ai.data(), ax.data(), cp,
                   ir.data(), xr.data(), 256, slice, cfirst, threshold, 2, 1);
        const auto rvv_status = GB_select_phase2_entry_rvv(ap, ai.data(), ax.data(), cp,
                   iv.data(), xv.data(), 256, slice, cfirst, threshold, 2, 1);
        if (scalar_status != rvv_status || scalar_status != GrB_SUCCESS) {
            std::fprintf(stderr, "entry threshold=%g scalar status=%d rvv status=%d\n",
                         threshold, int(scalar_status), int(rvv_status));
            return 1;
        }
        for (size_t p = 0; p < ir.size(); ++p)
            if (ir[p] != iv[p] || xr[p] != xv[p]) {
                std::fprintf(stderr, "entry threshold=%g p=%zu i=%ld/%ld x=%g/%g\n",
                             threshold, p, ir[p], iv[p], xr[p], xv[p]);
                return 1;
            }
    }
}
