#include "kernel.h"
#include <riscv_vector.h>

void GB_add_full_32_rvv(const uint32_t *Bp32, const uint64_t *Bp64,
                        const int32_t *Bi32, const int64_t *Bi64,
                        const uint32_t *Bh32, const uint64_t *Bh64,
                        const double *Bx, const double *Ax, double *Cx,
                        const int64_t *slice, int ntasks, int, int64_t vlen,
                        bool A_iso, bool B_iso, int64_t cnz, int, double) {
    const double a0 = A_iso ? Ax[0] : 0;
    for (int64_t p = 0; p < cnz;) {
        size_t vl = __riscv_vsetvl_e64m1(cnz - p);
        vfloat64m1_t a = A_iso ? __riscv_vfmv_v_f_f64m1(a0, vl)
                              : __riscv_vle64_v_f64m1(Ax + p, vl);
        __riscv_vse64_v_f64m1(Cx + p, a, vl);
        p += vl;
    }
    for (int tid = 0; tid < ntasks; ++tid) {
        for (int64_t k = slice[tid]; k <= slice[ntasks + tid]; ++k) {
            int64_t begin = k == slice[tid] ? slice[2 * ntasks + tid]
                                             : (Bp32 ? Bp32[k] : Bp64[k]);
            int64_t end = k == slice[ntasks + tid]
                              ? slice[2 * ntasks + tid + 1]
                              : (Bp32 ? Bp32[k + 1] : Bp64[k + 1]);
            if (k == slice[tid]) {
                int64_t next = Bp32 ? Bp32[k + 1] : Bp64[k + 1];
                if (end > next) end = next;
            }
            const int64_t j = Bh32 ? Bh32[k] : (Bh64 ? Bh64[k] : k);
            // Strictly increasing rows prove distinct destinations. Unsorted
            // rows, including duplicates, retain the original pB order.
            bool ordered_unique = true;
            for (int64_t x = begin + 1; x < end; ++x)
                if ((Bi32 ? Bi32[x] : Bi64[x]) <=
                    (Bi32 ? Bi32[x - 1] : Bi64[x - 1])) {
                    ordered_unique = false;
                    break;
                }
            if (!ordered_unique) {
                for (int64_t pB = begin; pB < end; ++pB) {
                    int64_t p = j * vlen + (Bi32 ? Bi32[pB] : Bi64[pB]);
                    Cx[p] = Ax[A_iso ? 0 : p] + Bx[B_iso ? 0 : pB];
                }
                continue;
            }
            for (int64_t pB = begin; pB < end;) {
                size_t vl = __riscv_vsetvl_e64m1(end - pB);
                vuint64m1_t idx = Bi32
                    ? __riscv_vwcvtu_x_x_v_u64m1(__riscv_vle32_v_u32mf2(
                          reinterpret_cast<const uint32_t *>(Bi32 + pB), vl), vl)
                    : __riscv_vle64_v_u64m1(
                          reinterpret_cast<const uint64_t *>(Bi64 + pB), vl);
                idx = __riscv_vadd_vx_u64m1(idx, j * vlen, vl);
                vfloat64m1_t a = A_iso ? __riscv_vfmv_v_f_f64m1(a0, vl)
                    : __riscv_vluxei64_v_f64m1(Ax,
                        __riscv_vsll_vx_u64m1(idx, 3, vl), vl);
                vfloat64m1_t b = B_iso ? __riscv_vfmv_v_f_f64m1(Bx[0], vl)
                    : __riscv_vle64_v_f64m1(Bx + pB, vl);
                __riscv_vsuxei64_v_f64m1(Cx,
                    __riscv_vsll_vx_u64m1(idx, 3, vl),
                    __riscv_vfadd_vv_f64m1(a, b, vl), vl);
                pB += vl;
            }
        }
    }
}
