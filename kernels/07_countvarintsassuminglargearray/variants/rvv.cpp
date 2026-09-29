#include "kernel.h"

#include <cassert>
#include <riscv_vector.h>

int CountVarintsAssumingLargeArray_rvv(const char *ptr, const char *end) {
    const int length = end - ptr;
    assert(length >= 8);
    int count = length;
    size_t remaining = static_cast<size_t>(length);
    while (remaining) {
        const size_t vl = __riscv_vsetvl_e8m1(remaining);
        const auto bytes = __riscv_vle8_v_u8m1(
            reinterpret_cast<const uint8_t *>(ptr), vl);
        const auto continuation = __riscv_vmsne_vx_u8m1_b8(
            __riscv_vand_vx_u8m1(bytes, 0x80, vl), 0, vl);
        count -= static_cast<int>(__riscv_vcpop_m_b8(continuation, vl));
        ptr += vl;
        remaining -= vl;
    }
    return count;
}
