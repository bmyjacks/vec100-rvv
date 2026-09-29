#include "kernel.h"

#include <riscv_vector.h>

uint32_t findPosFirstLast_c_rvv(const int16_t *coeff, std::intptr_t stride,
                                const uint16_t *scan) {
    // A coding group is 4x4. Its scan table is a permutation of 0..15;
    // the caller supplies a transform stride (4, 8, 16 or 32 in x265).
    constexpr size_t count = SCAN_SET_SIZE;
    // m2 holds all 16 lanes even at VLEN=128; the gather's EEW32 offsets
    // have EMUL=m4.
    const auto order = __riscv_vle16_v_u16m2(scan, count);
    const auto indices = __riscv_vzext_vf2_u32m4(order, count);
    const auto rows = __riscv_vsrl_vx_u32m4(indices, 2, count);
    const auto columns = __riscv_vand_vx_u32m4(indices, 3, count);
    const auto offsets = __riscv_vsll_vx_u32m4(
        __riscv_vadd_vv_u32m4(
            __riscv_vmul_vx_u32m4(rows, static_cast<uint32_t>(stride), count),
            columns, count), 1, count);
    const auto values = __riscv_vluxei32_v_i16m2(coeff, offsets, count);
    const auto nonzero = __riscv_vmsne_vx_i16m2_b8(values, 0, count);
    const long first = __riscv_vfirst_m_b8(nonzero, count);
    if (first < 0) {
        // Upstream only defines the low byte (16) for an all-zero group.
        // Match the extracted scalar's current packed bits as well.
        return 0xffffff10u;
    }

    const auto positions = __riscv_vid_v_u16m2(count);
    const auto selected = __riscv_vmerge_vvm_u16m2(
        __riscv_vmv_v_x_u16m2(0, count), positions, nonzero, count);
    const auto last_vec = __riscv_vredmaxu_vs_u16m2_u16m1(
        selected, __riscv_vmv_v_x_u16m1(0, count), count);
    const uint32_t last = __riscv_vmv_x_s_u16m1_u16(last_vec);

    // Only the parity of the signed sum enters bit 31. Zeros before the
    // first and after the last do not affect that parity.
    const auto parity = __riscv_vand_vx_u16m2(
        __riscv_vreinterpret_v_i16m2_u16m2(values), 1, count);
    const auto sum = __riscv_vredsum_vs_u16m2_u16m1(
        parity, __riscv_vmv_v_x_u16m1(0, count), count);
    return ((__riscv_vmv_x_s_u16m1_u16(sum) & 1u) << 31) |
           (last << 8) | static_cast<uint32_t>(first);
}
