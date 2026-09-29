#include "kernel.h"

#include <riscv_vector.h>

void lz4_renorm_hash_table_rvv(LZ4_hash_table_view *dict, U32 delta) {
    int i = 0;
    while (i < LZ4_HASH_SIZE_U32) {
        const size_t vl = __riscv_vsetvl_e32m1(LZ4_HASH_SIZE_U32 - i);
        const vuint32m1_t entry =
            __riscv_vle32_v_u32m1(dict->hashTable + i, vl);
        const vbool32_t clear = __riscv_vmsltu_vx_u32m1_b32(entry, delta, vl);
        const vuint32m1_t shifted = __riscv_vsub_vx_u32m1(entry, delta, vl);
        const vuint32m1_t result =
            __riscv_vmerge_vxm_u32m1(shifted, 0, clear, vl);
        __riscv_vse32_v_u32m1(dict->hashTable + i, result, vl);
        i += static_cast<int>(vl);
    }
}
