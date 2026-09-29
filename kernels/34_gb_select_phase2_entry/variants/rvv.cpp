#include "kernel.h"
#include <riscv_vector.h>

GrB_Info GB_select_phase2_entry_rvv(const uint64_t *Ap, const int64_t *Ai,
                                    const double *Ax, const uint64_t *Cp,
                                    int64_t *Ci, double *Cx, int64_t,
                                    const int64_t *slice,
                                    const uint64_t *Cp_kfirst,
                                    double threshold, int ntasks, int) {
    for (int tid = 0; tid < ntasks; ++tid)
        for (int64_t k = slice[tid]; k <= slice[ntasks + tid]; ++k) {
            int64_t begin = k == slice[tid] ? slice[2 * ntasks + tid] : Ap[k];
            int64_t end = k == slice[ntasks + tid] ? slice[2 * ntasks + tid + 1] : Ap[k + 1];
            if (k == slice[tid] && end > int64_t(Ap[k + 1])) end = Ap[k + 1];
            int64_t out = k == slice[tid] ? Cp_kfirst[tid] : Cp[k];
            for (int64_t p = begin; p < end;) {
                size_t vl = __riscv_vsetvl_e64m1(end - p);
                vfloat64m1_t x = __riscv_vle64_v_f64m1(Ax + p, vl);
                vbool64_t keep = __riscv_vmfgt_vf_f64m1_b64(x, threshold, vl);
                size_t count = __riscv_vcpop_m_b64(keep, vl);
                if (count) {
                    vuint64m1_t indices = __riscv_vle64_v_u64m1(
                        reinterpret_cast<const uint64_t *>(Ai + p), vl);
                    __riscv_vse64_v_u64m1(
                        reinterpret_cast<uint64_t *>(Ci + out),
                        __riscv_vcompress_vm_u64m1(indices, keep, vl), count);
                    __riscv_vse64_v_f64m1(Cx + out,
                        __riscv_vcompress_vm_f64m1(x, keep, vl), count);
                    out += count;
                }
                p += vl;
            }
        }
    return GrB_SUCCESS;
}
