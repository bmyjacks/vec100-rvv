#include "kernel.h"
#include <riscv_vector.h>

void GB_emult_02a_template_rvv(
    const int32_t *Ai32, const int64_t *Ai64,
    const uint32_t *Ap32, const uint64_t *Ap64,
    const uint32_t *Ah32, const uint64_t *Ah64,
    const uint32_t *Cp32, const uint64_t *Cp64,
    const double *Ax, const double *Bx, const int8_t *Bb,
    double *Cx, int32_t *Ci32, int64_t *Ci64,
    const int64_t *kfirst, const int64_t *klast,
    const int64_t *pstart, const int64_t *Cp_kfirst,
    int64_t vlen, int ntasks, int, bool A_iso, bool B_iso) {
    for (int tid = 0; tid < ntasks; ++tid)
        for (int64_t k = kfirst[tid]; k <= klast[tid]; ++k) {
            int64_t j = Ah32 ? Ah32[k] : (Ah64 ? Ah64[k] : k);
            int64_t begin = k == kfirst[tid] ? pstart[tid]
                                                  : (Ap32 ? Ap32[k] : Ap64[k]);
            int64_t end = k == klast[tid] ? pstart[tid + 1]
                                                 : (Ap32 ? Ap32[k + 1] : Ap64[k + 1]);
            if (k == kfirst[tid]) {
                int64_t next = Ap32 ? Ap32[k + 1] : Ap64[k + 1];
                if (end > next) end = next;
            }
            int64_t pC = k == kfirst[tid] ? Cp_kfirst[tid]
                                                  : (Cp32 ? Cp32[k] : Cp64[k]);
            for (int64_t p = begin; p < end;) {
                size_t vl = __riscv_vsetvl_e64m1(end - p);
                vuint64m1_t idx = Ai32
                    ? __riscv_vwcvtu_x_x_v_u64m1(__riscv_vle32_v_u32mf2(
                        reinterpret_cast<const uint32_t *>(Ai32 + p), vl), vl)
                    : __riscv_vle64_v_u64m1(
                        reinterpret_cast<const uint64_t *>(Ai64 + p), vl);
                vuint64m1_t loc = __riscv_vadd_vx_u64m1(idx, j * vlen, vl);
                vuint8mf8_t bits = __riscv_vluxei64_v_u8mf8(
                    reinterpret_cast<const uint8_t *>(Bb), loc, vl);
                vbool64_t keep = __riscv_vmsne_vx_u8mf8_b64(bits, 0, vl);
                size_t count = __riscv_vcpop_m_b64(keep, vl);
                if (count) {
                    vuint64m1_t packed = __riscv_vcompress_vm_u64m1(idx, keep, vl);
                    if (Ci64) __riscv_vse64_v_u64m1(
                        reinterpret_cast<uint64_t *>(Ci64 + pC), packed, count);
                    else {
                        vuint32mf2_t narrow = __riscv_vnsrl_wx_u32mf2(packed, 0, count);
                        __riscv_vse32_v_u32mf2(
                            reinterpret_cast<uint32_t *>(Ci32 + pC), narrow, count);
                    }
                    vfloat64m1_t a = A_iso ? __riscv_vfmv_v_f_f64m1(Ax[0], vl)
                                                    : __riscv_vle64_v_f64m1(Ax + p, vl);
                    vfloat64m1_t b = B_iso ? __riscv_vfmv_v_f_f64m1(Bx[0], vl)
                        : __riscv_vluxei64_v_f64m1_m(keep, Bx,
                            __riscv_vsll_vx_u64m1(loc, 3, vl), vl);
                    vfloat64m1_t product = __riscv_vfmul_vv_f64m1(a, b, vl);
                    __riscv_vse64_v_f64m1(Cx + pC,
                        __riscv_vcompress_vm_f64m1(product, keep, vl), count);
                    pC += count;
                }
                p += vl;
            }
        }
}
