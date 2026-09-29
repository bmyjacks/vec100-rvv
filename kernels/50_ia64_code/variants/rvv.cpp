#include "kernel.h"

#include <riscv_vector.h>

static void scalar_bundle(uint8_t *buffer, uint32_t now_pos, bool is_encoder,
                          uint8_t mask) {
    for (unsigned slot = 0; slot < 3; ++slot) {
        if (!(mask & (1U << slot))) continue;
        const unsigned bit_pos = 5 + 41 * slot;
        const unsigned byte_pos = bit_pos / 8;
        const unsigned bit_res = bit_pos % 8;
        uint64_t instruction = 0;
        for (unsigned j = 0; j < 6; ++j)
            instruction |= uint64_t(buffer[byte_pos + j]) << (8 * j);
        const uint64_t norm = instruction >> bit_res;
        if (((norm >> 37) & 15) != 5 || ((norm >> 9) & 7)) continue;
        const uint32_t src = uint32_t(((norm >> 13) & 0xfffff) |
                                      (((norm >> 36) & 1) << 20)) << 4;
        const uint32_t dest = (is_encoder ? now_pos + src : src - now_pos) >> 4;
        const uint64_t field = uint64_t(0x8fffff) << 13;
        const uint64_t bits = ((uint64_t(dest) & 0xfffff) << 13) |
                              ((uint64_t(dest) & 0x100000) << 16);
        instruction = (instruction & ~(field << bit_res)) | (bits << bit_res);
        for (unsigned j = 0; j < 6; ++j)
            buffer[byte_pos + j] = uint8_t(instruction >> (8 * j));
    }
}

// XZ 5.8.4 src/liblzma/simple/ia64.c; the same slot layout is in upstream main.
// Process independent 16-byte bundles in vectors. Slots within one bundle
// share bytes 5 and 10, so read/commit one slot at a time, in source order.
size_t ia64_code_rvv(void *simple, uint32_t now_pos, bool is_encoder,
                          uint8_t *buffer, size_t size) {
    (void)simple;
    static const uint8_t branch_table[32] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 4, 6, 6, 0, 0, 7, 7, 4, 4, 0, 0, 4, 4, 0, 0
    };
    const size_t bundles = (size & ~(size_t)15) / 16;
    if (bundles == 1) {
        scalar_bundle(buffer, now_pos, is_encoder,
                      branch_table[buffer[0] & 31]);
        return 16;
    }
    constexpr uint64_t field = uint64_t(0x8fffff) << 13;

    for (size_t first = 0; first < bundles;) {
        // Fixed stack space also bounds VL on machines with arbitrarily large VLEN.
        const size_t remaining = bundles - first;
        const size_t vl = __riscv_vsetvl_e64m2(remaining < 4 ? remaining : 4);
        uint64_t norms[4] = {}, bases[4] = {}, results[4], valid[4];
        for (unsigned slot = 0; slot < 3; ++slot) {
            const unsigned bit_pos = 5 + 41 * slot;
            const unsigned byte_pos = bit_pos / 8;
            const unsigned bit_res = bit_pos % 8;
            for (size_t lane = 0; lane < vl; ++lane) {
                const size_t offset = (first + lane) * 16;
                if (!(branch_table[buffer[offset] & 31] & (1U << slot))) {
                    norms[lane] = 0;
                    continue;
                }
                uint64_t instruction = 0;
                for (unsigned j = 0; j < 6; ++j)
                    instruction |= uint64_t(buffer[offset + byte_pos + j]) << (8 * j);
                norms[lane] = instruction >> bit_res;
                bases[lane] = uint32_t(now_pos + uint32_t(offset));
            }

            const vuint64m2_t n = __riscv_vle64_v_u64m2(norms, vl);
            const vbool32_t branch = __riscv_vmseq_vx_u64m2_b32(
                __riscv_vand_vx_u64m2(__riscv_vsrl_vx_u64m2(n, 37, vl), 15, vl), 5, vl);
            const vbool32_t reserved = __riscv_vmseq_vx_u64m2_b32(
                __riscv_vand_vx_u64m2(__riscv_vsrl_vx_u64m2(n, 9, vl), 7, vl), 0, vl);
            const vbool32_t match = __riscv_vmand_mm_b32(branch, reserved, vl);
            const vuint64m2_t src = __riscv_vsll_vx_u64m2(
                __riscv_vor_vv_u64m2(
                    __riscv_vand_vx_u64m2(__riscv_vsrl_vx_u64m2(n, 13, vl), 0xfffff, vl),
                    __riscv_vsll_vx_u64m2(
                        __riscv_vand_vx_u64m2(__riscv_vsrl_vx_u64m2(n, 36, vl), 1, vl),
                        20, vl), vl), 4, vl);
            const vuint64m2_t pos = __riscv_vle64_v_u64m2(bases, vl);
            const vuint64m2_t dest = is_encoder
                ? __riscv_vadd_vv_u64m2(src, pos, vl)
                : __riscv_vsub_vv_u64m2(src, pos, vl);
            const vuint64m2_t bits = __riscv_vand_vx_u64m2(
                __riscv_vsrl_vx_u64m2(dest, 4, vl), 0x1fffff, vl);
            __riscv_vse64_v_u64m2(results, bits, vl);
            __riscv_vse64_v_u64m2(valid,
                __riscv_vmerge_vxm_u64m2(__riscv_vmv_v_x_u64m2(0, vl), 1, match, vl), vl);

            for (size_t lane = 0; lane < vl; ++lane) {
                const size_t offset = (first + lane) * 16;
                if (!(branch_table[buffer[offset] & 31] & (1U << slot)) || !valid[lane])
                    continue;
                // A whole six-byte store would revert a previous slot's shared
                // boundary byte. Touch only this slot's 21 branch-address bits.
                uint64_t instruction = 0;
                for (unsigned j = 0; j < 6; ++j)
                    instruction |= uint64_t(buffer[offset + byte_pos + j]) << (8 * j);
                const uint64_t dest_bits = ((results[lane] & 0xfffff) << 13) |
                                           ((results[lane] & 0x100000) << 16);
                instruction = (instruction & ~(field << bit_res)) |
                              (dest_bits << bit_res);
                for (unsigned j = 0; j < 6; ++j)
                    buffer[offset + byte_pos + j] = uint8_t(instruction >> (8 * j));
            }
        }
        first += vl;
    }
    return size & ~(size_t)15;
}
