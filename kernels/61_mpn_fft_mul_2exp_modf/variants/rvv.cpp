#include "kernel.h"

#include <cstdint>
#include <memory>
#include <riscv_vector.h>

namespace {

// Identical twiddle to the pinned GMP extraction. Each lane gets its own
// n+1-limb destination: the caller's tp cannot hold simultaneous twiddles.
void mul_2exp_modF(mp_ptr r, mp_srcptr a, mp_bitcnt_t d, mp_size_t n) {
    const unsigned int sh = d % GMP_NUMB_BITS;
    mp_size_t m = d / GMP_NUMB_BITS;
    mp_limb_t cc, rd;
    if (m >= n) {
        m -= n;
        if (sh != 0) {
            mpn_lshift(r, a + n - m, m + 1, sh);
            rd = r[m];
            cc = mpn_lshiftc(r + m, a, n - m, sh);
        } else {
            MPN_COPY(r, a + n - m, m);
            rd = a[n];
            mpn_com(r + m, a, n - m);
            cc = 0;
        }
        r[n] = 0;
        ++cc;
        MPN_INCR_U(r, n + 1, cc);
        ++rd;
        cc = rd + (rd == 0);
        r = r + m + (rd == 0);
        MPN_INCR_U(r, n + 1 - m - (rd == 0), cc);
    } else {
        if (sh != 0) {
            mpn_lshiftc(r, a + n - m, m + 1, sh);
            rd = ~r[m];
            cc = mpn_lshift(r + m, a, n - m, sh);
        } else {
            mpn_com(r, a + n - m, m + 1);
            rd = a[n];
            MPN_COPY(r + m, a, n - m);
            cc = 0;
        }
        if (m != 0) {
            if (cc-- == 0)
                cc = mpn_add_1(r, r, n, CNST_LIMB(1));
            cc = mpn_sub_1(r, r, m, cc) + 1;
        }
        r[n] = 2;
        MPN_DECR_U(r + m, n - m + 1, cc);
        MPN_DECR_U(r + m, n - m + 1, rd);
        if (UNLIKELY((r[n] -= 2) != 0)) {
            mp_limb_t cy = -r[n];
            r[n] = 0;
            MPN_INCR_U(r, n + 1, cy);
        }
    }
}

// Indexed addresses let adjacent vector lanes refer to separate Ap entries;
// within a lane the limb index increases and carries remain serial. Offsets
// are 64-bit address differences (mod 2^64), including for unrelated arrays.
void butterflies(mp_ptr *Ap, mp_size_t n, mp_ptr tw,
                 size_t lanes, const uint64_t *aoff, const uint64_t *boff) {
    const auto vl = __riscv_vsetvl_e64m1(lanes);
    const vuint64m1_t ai = __riscv_vle64_v_u64m1(aoff, vl);
    const vuint64m1_t bi = __riscv_vle64_v_u64m1(boff, vl);
    const vuint64m1_t ti = __riscv_vid_v_u64m1(vl);
    const vuint64m1_t to = __riscv_vmul_vx_u64m1(ti, (n + 1) * sizeof(mp_limb_t), vl);

    vuint64m1_t add_cy = __riscv_vmv_v_x_u64m1(0, vl);
    vuint64m1_t sub_cy = add_cy;
    for (mp_size_t i = 0; i < n; ++i) {
        const size_t delta = i * sizeof(mp_limb_t);
        const vuint64m1_t aa = __riscv_vadd_vx_u64m1(ai, delta, vl);
        const vuint64m1_t bb = __riscv_vadd_vx_u64m1(bi, delta, vl);
        const vuint64m1_t tt = __riscv_vadd_vx_u64m1(to, delta, vl);
        const vuint64m1_t a = __riscv_vluxei64_v_u64m1(Ap[0], aa, vl);
        const vuint64m1_t b = __riscv_vluxei64_v_u64m1(tw, tt, vl);

        const vuint64m1_t sum = __riscv_vadd_vv_u64m1(a, b, vl);
        const vuint64m1_t out_add = __riscv_vadd_vv_u64m1(sum, add_cy, vl);
        const vbool64_t cy0 = __riscv_vmsltu_vv_u64m1_b64(sum, a, vl);
        const vbool64_t cy1 = __riscv_vmsltu_vv_u64m1_b64(out_add, sum, vl);
        add_cy = __riscv_vmerge_vxm_u64m1(__riscv_vmerge_vxm_u64m1(
            __riscv_vmv_v_x_u64m1(0, vl), 1, cy0, vl), 1, cy1, vl);

        const vuint64m1_t diff = __riscv_vsub_vv_u64m1(a, b, vl);
        const vuint64m1_t out_sub = __riscv_vsub_vv_u64m1(diff, sub_cy, vl);
        const vbool64_t borrow0 = __riscv_vmsltu_vv_u64m1_b64(a, b, vl);
        const vbool64_t borrow1 = __riscv_vmsltu_vv_u64m1_b64(diff, sub_cy, vl);
        sub_cy = __riscv_vmerge_vxm_u64m1(__riscv_vmerge_vxm_u64m1(
            __riscv_vmv_v_x_u64m1(0, vl), 1, borrow0, vl), 1, borrow1, vl);

        __riscv_vsuxei64_v_u64m1(Ap[0], bb, out_sub, vl);
        __riscv_vsuxei64_v_u64m1(Ap[0], aa, out_add, vl);
    }

    const size_t delta = n * sizeof(mp_limb_t);
    const vuint64m1_t aa = __riscv_vadd_vx_u64m1(ai, delta, vl);
    const vuint64m1_t bb = __riscv_vadd_vx_u64m1(bi, delta, vl);
    const vuint64m1_t tt = __riscv_vadd_vx_u64m1(to, delta, vl);
    const vuint64m1_t a = __riscv_vluxei64_v_u64m1(Ap[0], aa, vl);
    const vuint64m1_t b = __riscv_vluxei64_v_u64m1(tw, tt, vl);
    const vuint64m1_t ac = __riscv_vadd_vv_u64m1(__riscv_vadd_vv_u64m1(a, b, vl), add_cy, vl);
    const vuint64m1_t sc = __riscv_vsub_vv_u64m1(__riscv_vsub_vv_u64m1(a, b, vl), sub_cy, vl);
    const vbool64_t nonzero = __riscv_vmsne_vx_u64m1_b64(ac, 0, vl);
    const vbool64_t negative = __riscv_vmslt_vx_i64m1_b64(__riscv_vreinterpret_v_u64m1_i64m1(sc), 0, vl);
    const vuint64m1_t ax = __riscv_vmerge_vvm_u64m1(
        __riscv_vmv_v_x_u64m1(0, vl), __riscv_vsub_vx_u64m1(ac, 1, vl), nonzero, vl);
    // -sc for negative lanes; the rest remain zero.
    const vuint64m1_t sub_x = __riscv_vmerge_vvm_u64m1(__riscv_vmv_v_x_u64m1(0, vl),
        __riscv_vsub_vv_u64m1(__riscv_vmv_v_x_u64m1(0, vl), sc, vl), negative, vl);
    __riscv_vsuxei64_v_u64m1(Ap[0], aa, __riscv_vsub_vv_u64m1(ac, ax, vl), vl);
    __riscv_vsuxei64_v_u64m1(Ap[0], bb, __riscv_vadd_vv_u64m1(sc, sub_x, vl), vl);

    // Modular corrections: subtract ax from the sum and add sub_x to the
    // difference. Each lane propagates its own borrow/carry through n+1 limbs.
    vuint64m1_t dec = ax, inc_cy = sub_x;
    for (mp_size_t i = 0; i <= n; ++i) {
        const size_t off = i * sizeof(mp_limb_t);
        const vuint64m1_t da = __riscv_vadd_vx_u64m1(ai, off, vl);
        const vuint64m1_t db = __riscv_vadd_vx_u64m1(bi, off, vl);
        const vuint64m1_t u = __riscv_vluxei64_v_u64m1(Ap[0], da, vl);
        const vuint64m1_t v = __riscv_vluxei64_v_u64m1(Ap[0], db, vl);
        const vuint64m1_t d = __riscv_vsub_vv_u64m1(u, dec, vl);
        const vuint64m1_t s = __riscv_vadd_vv_u64m1(v, inc_cy, vl);
        const vbool64_t borrow = __riscv_vmsltu_vv_u64m1_b64(u, dec, vl);
        const vbool64_t carry = __riscv_vmsltu_vv_u64m1_b64(s, inc_cy, vl);
        dec = __riscv_vmerge_vxm_u64m1(__riscv_vmv_v_x_u64m1(0, vl), 1, borrow, vl);
        inc_cy = __riscv_vmerge_vxm_u64m1(__riscv_vmv_v_x_u64m1(0, vl), 1, carry, vl);
        __riscv_vsuxei64_v_u64m1(Ap[0], da, d, vl);
        __riscv_vsuxei64_v_u64m1(Ap[0], db, s, vl);
    }
}

void fft(mp_ptr *Ap, mp_size_t K, int **ll, mp_size_t omega,
         mp_size_t n, mp_size_t inc, mp_ptr tp, mp_ptr tw,
         uint64_t *aoff, uint64_t *boff) {
    if (K == 2) {
        MPN_COPY(tp, Ap[0], n + 1);
        mpn_add_n(Ap[0], Ap[0], Ap[inc], n + 1);
        const mp_limb_t cy = mpn_sub_n(Ap[inc], tp, Ap[inc], n + 1);
        if (Ap[0][n] > 1) {
            const mp_limb_t cc = Ap[0][n] - 1;
            Ap[0][n] = 1;
            MPN_DECR_U(Ap[0], n + 1, cc);
        }
        if (cy) {
            const mp_limb_t cc = ~Ap[inc][n] + 1;
            Ap[inc][n] = 0;
            MPN_INCR_U(Ap[inc], n + 1, cc);
        }
        return;
    }
    const mp_size_t half = K >> 1;
    int *lk = *ll;
    fft(Ap, half, ll - 1, 2 * omega, n, inc * 2, tp, tw, aoff, boff);
    fft(Ap + inc, half, ll - 1, 2 * omega, n, inc * 2, tp, tw, aoff, boff);
    for (mp_size_t j = 0; j < half;) {
        const size_t lanes = __riscv_vsetvl_e64m1(half - j);
        const uintptr_t base = reinterpret_cast<uintptr_t>(Ap[0]);
        for (size_t k = 0; k < lanes; ++k) {
            mp_ptr *pair = Ap + 2 * (j + k) * inc;
            mul_2exp_modF(tw + k * (n + 1), pair[inc], lk[2 * (j + k)] * omega, n);
            aoff[k] = reinterpret_cast<uintptr_t>(pair[0]) - base;
            boff[k] = reinterpret_cast<uintptr_t>(pair[inc]) - base;
        }
        butterflies(Ap, n, tw, lanes, aoff, boff);
        j += lanes;
    }
}
} // namespace

void mpn_fft_fft_rvv(mp_ptr *Ap, mp_size_t K, int **ll, mp_size_t omega,
                          mp_size_t n, mp_size_t inc, mp_ptr tp) {
    if (K == 2) {
        fft(Ap, K, ll, omega, n, inc, tp, nullptr, nullptr, nullptr);
        return;
    }
    const size_t capacity = __riscv_vsetvlmax_e64m1();
    std::unique_ptr<mp_limb_t[]> tw(new mp_limb_t[capacity * (n + 1)]);
    std::unique_ptr<uint64_t[]> offsets(new uint64_t[2 * capacity]);
    fft(Ap, K, ll, omega, n, inc, tp, tw.get(), offsets.get(),
        offsets.get() + capacity);
}
