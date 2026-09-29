#include "kernel.h"

#include <algorithm>
#include <cmath>
#include <riscv_vector.h>
#include <vector>

void whisper_mel_filterbank_projection_rvv(const float *fft_out,
                                           const float *filters, float *mel,
                                           int n_fft, int n_mel, int mel_len,
                                           int i) {
    // Compile with FP contraction/implicit vectorization disabled: materialize
    // the individual FP32 products before the ordered FP32/FP64 reduction.
    const int groups = n_fft / 4;
    std::vector<float> products(n_fft > 0 ? n_fft : 0);
    for (int j = 0; j < n_mel; ++j) {
        const float *filter = n_fft ? filters + j * n_fft : filters;
        for (int k = 0; k < n_fft;) {
            size_t vl = __riscv_vsetvl_e32m1(n_fft - k);
            vfloat32m1_t a = __riscv_vle32_v_f32m1(fft_out + k, vl);
            vfloat32m1_t b = __riscv_vle32_v_f32m1(filter + k, vl);
            __riscv_vse32_v_f32m1(products.data() + k,
                                  __riscv_vfmul_vv_f32m1(a, b, vl), vl);
            k += vl;
        }
        double sum = 0.0;
        for (int g = 0; g < groups; ++g) {
            int k = 4 * g;
            sum += products[k] + products[k + 1] +
                   products[k + 2] + products[k + 3];
        }
        for (int k = groups * 4; k < n_fft; ++k) {
            sum += products[k];
        }
        sum = log10(std::max(sum, 1e-10));
        mel[j * mel_len + i] = sum;
    }
}
