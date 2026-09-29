#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

void bli_zzpackm_1er_generic_ref_rvv(conj_t, pack_t, dim_t, dim_t, dim_t,
                                     dim_t, dim_t, const void *, const void *,
                                     inc_t, inc_t, void *, inc_t,
                                     const void *, const cntx_t *);
#ifndef RVV_STANDALONE
void bli_zzpackm_1er_generic_ref_wrapper(conj_t, pack_t, dim_t, dim_t, dim_t,
                                        dim_t, dim_t, const void *, const void *,
                                        inc_t, inc_t, void *, inc_t,
                                        const void *, const cntx_t *);
#endif
static uint32_t seed = 0x04b115ea;
static uint32_t random_word() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

static void check(dim_t cdim, dim_t cdim_max, dim_t bc, dim_t n, dim_t n_max,
                  inc_t inca, inc_t lda, inc_t ldp, bool one_e, bool conj,
                  bool alias, int trial) {
    const dim_t cols = 2 * n_max;
    const size_t p_count = size_t(cols * ldp + 32);
    const size_t a_count = size_t(2 * (n * lda + cdim * inca + 2));
    const size_t p_off = alias ? 0 : a_count + 16;
    const size_t total = (alias ? (a_count > p_count ? a_count : p_count)
                                : p_off + p_count) + 32;
    std::vector<double> original(total);
    for (auto &v : original) v = (int(random_word() % 4001) - 2000) / 16.0;
    if (trial % 31 == 0 && !original.empty()) original[0] = std::numeric_limits<double>::quiet_NaN();
    if (trial % 37 == 0 && original.size() > 3) original[3] = std::numeric_limits<double>::infinity();
    if (trial % 41 == 0 && original.size() > 6) original[6] = std::numeric_limits<double>::denorm_min();
    if (alias && n && cdim) {
        // Output aliases the source with a forward dependence across elements.
        original[0] = -0.0;
    }
    double kappa[2] = {1.5, -0.75};
    if (trial % 9 == 0) kappa[0] = 0.0;
    if (trial % 11 == 0) kappa[1] = -0.0;
    auto expected = original, actual = original;
    const auto schema = one_e ? BLIS_PACKED_PANELS_1E : BLIS_PACKED_PANELS_1R;
    const auto conjugate = conj ? BLIS_CONJUGATE : BLIS_NO_CONJUGATE;
    const auto call = [&](std::vector<double> &v, bool vector) {
        if (vector)
            bli_zzpackm_1er_generic_ref_rvv(conjugate, schema, cdim, cdim_max, bc,
                                              n, n_max, kappa, v.data(), inca, lda,
                                              v.data() + p_off, ldp, nullptr, nullptr);
#ifndef RVV_STANDALONE
        else
            bli_zzpackm_1er_generic_ref_wrapper(conjugate, schema, cdim, cdim_max, bc,
                                                 n, n_max, kappa, v.data(), inca, lda,
                                                 v.data() + p_off, ldp, nullptr, nullptr);
#endif
    };
#ifndef RVV_STANDALONE
    call(expected, false);
#endif
    call(actual, true);
#ifndef RVV_STANDALONE
    for (size_t i = 0; i < total; ++i)
        if (std::memcmp(&actual[i], &expected[i], sizeof(double))) {
            std::fprintf(stderr, "seed=0x04b115ea cdim=%ld max=%ld bc=%ld n=%ld max=%ld inca=%ld lda=%ld ldp=%ld 1e=%d conj=%d alias=%d trial=%d index=%zu actual=%a expected=%a\n",
                         cdim, cdim_max, bc, n, n_max, inca, lda, ldp,
                         one_e, conj, alias, trial, i, actual[i], expected[i]);
            std::exit(1);
        }
#endif
}

int main() {
    int trial = 0;
    for (dim_t cdim : {0L, 1L, 2L, 3L, 4L, 7L, 8L, 9L, 16L, 17L, 33L})
        for (dim_t bc : {1L, 2L, 3L})
            for (bool one_e : {false, true})
                for (bool conj : {false, true})
                    for (bool alias : {false, true}) {
                        const dim_t n = trial % 7 == 0 ? 0 : (trial % 5 + 1);
                        const dim_t n_max = n + trial % 3;
                        const dim_t cdim_max = cdim + trial % 4;
                        const inc_t inca = trial % 4 == 0 ? 2 : 1;
                        const inc_t lda = cdim * inca + 2;
                        const inc_t ldp = (one_e ? 2 : 1) * cdim_max * bc + 7;
                        check(cdim, cdim_max, bc, n, n_max, inca, lda, ldp,
                              one_e, conj, alias, trial++);
                    }
    for (int j = 0; j < 500; ++j) {
        const dim_t c = random_word() % 71;
        const dim_t bc = random_word() % 4 + 1;
        const dim_t nc = random_word() % 6;
        const bool e = random_word() & 1;
        const inc_t inca = random_word() % 3 + 1;
        check(c, c + random_word() % 4, bc, nc, nc + random_word() % 3,
              inca, c * inca + 2, (e ? 2 : 1) * (c + 3) * bc + 4,
              e, random_word() & 1, random_word() & 1, trial++);
    }
    std::puts("BLIS pack PASS (seed=0x04b115ea)");
}
