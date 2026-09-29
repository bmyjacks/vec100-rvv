#include "kernel.h"

#include <riscv_vector.h>

namespace simdjson {
namespace fallback {
namespace numberparsing {

bool is_made_of_eight_digits_fast_rvv(const uint8_t *chars) {
    // RVV's minimum VLEN for rv64gcv is 128 bits: eight e8,m1 lanes fit.
    // Request exactly eight; never read bytes beyond the predicate's window.
    const size_t vl = __riscv_vsetvl_e8m1(8);
    const vuint8m1_t bytes = __riscv_vle8_v_u8m1(chars, vl);
    const vuint8m1_t digits = __riscv_vsub_vx_u8m1(bytes, '0', vl);
    const vbool8_t valid = __riscv_vmsleu_vx_u8m1_b8(digits, 9, vl);
    return __riscv_vcpop_m_b8(valid, vl) == 8;
}

} // namespace numberparsing
} // namespace fallback
} // namespace simdjson
