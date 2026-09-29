#include "kernel.h"
#include <riscv_vector.h>

void ggml_compute_forward_group_norm_f32_bench_rvv(const float *input,
                                                     float *output, int64_t ne00,
                                                     int64_t ne01, int channels,
                                                     int groups, float eps) {
    int per = (channels + groups - 1) / groups;
    for (int g = 0; g < groups; ++g) {
        int start = g * per;
        int end = start + per < channels ? start + per : channels;
        int step = end - start;
        // ggml_float is double; preserve both levels of scalar summation.
        double sum = 0;
        for (int ch = start; ch < end; ++ch)
            for (int64_t row = 0; row < ne01; ++row) {
                const float *x = input + (ch * ne01 + row) * ne00;
                double sumr = 0;
                for (int64_t j = 0; j < ne00; ++j) sumr += (double)x[j];
                sum += sumr;
            }
        float mean = sum / (ne00 * ne01 * step);
        double sum2 = 0;
        for (int ch = start; ch < end; ++ch)
            for (int64_t row = 0; row < ne01; ++row) {
                const float *x = input + (ch * ne01 + row) * ne00;
                float *y = output + (ch * ne01 + row) * ne00;
                // Vectorize the centering; accumulate squares in source order
                // to retain the exact double-precision reduction contract.
                for (int64_t j = 0; j < ne00;) {
                    size_t vl = __riscv_vsetvl_e32m1(ne00 - j);
                    auto v = __riscv_vle32_v_f32m1(x + j, vl);
                    v = __riscv_vfsub_vf_f32m1(v, mean, vl);
                    __riscv_vse32_v_f32m1(y + j, v, vl);
                    j += vl;
                }
                double sumr = 0;
                for (int64_t j = 0; j < ne00; ++j) sumr += (double)(y[j] * y[j]);
                sum2 += sumr;
            }
        float variance = sum2 / (ne00 * ne01 * step);
        float scale = 1.0f / sqrtf(variance + eps);
        for (int ch = start; ch < end; ++ch)
            for (int64_t row = 0; row < ne01; ++row) {
                float *y = output + (ch * ne01 + row) * ne00;
                for (int64_t j = 0; j < ne00;) {
                    size_t vl = __riscv_vsetvl_e32m1(ne00 - j);
                    auto v = __riscv_vle32_v_f32m1(y + j, vl);
                    v = __riscv_vfmul_vf_f32m1(v, scale, vl);
                    __riscv_vse32_v_f32m1(y + j, v, vl);
                    j += vl;
                }
            }
    }
}
