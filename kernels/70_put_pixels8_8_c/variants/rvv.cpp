#include "kernel.h"

#include <riscv_vector.h>

void put_pixels8_8_c_rvv(uint8_t *block, const uint8_t *pixels, ptrdiff_t line_size,
                     int h) {
    if (h <= 0)
        return;

    const ptrdiff_t last = static_cast<ptrdiff_t>(h - 1) * line_size;
    const ptrdiff_t low = last < 0 ? last : 0;
    const ptrdiff_t high = (last > 0 ? last : 0) + 8;
    const uintptr_t a = reinterpret_cast<uintptr_t>(block + low);
    const uintptr_t b = reinterpret_cast<uintptr_t>(pixels + low);

    // Each scalar row reads four bytes, writes them, then reads four more.
    // Any cross-row or within-row overlap must retain that exact ordering.
    if (a < b + static_cast<uintptr_t>(high - low) &&
        b < a + static_cast<uintptr_t>(high - low)) {
        for (int i = 0; i < h; ++i) {
            *((pixel4 *)block) = AV_RN4P(pixels);
            *((pixel4 *)(block + 4 * sizeof(pixel))) =
                AV_RN4P(pixels + 4 * sizeof(pixel));
            pixels += line_size;
            block += line_size;
        }
        return;
    }

    for (int i = 0; i < h; ++i) {
        size_t x = 0;
        while (x < 8) {
            const size_t vl = __riscv_vsetvl_e8m1(8 - x);
            vuint8m1_t v = __riscv_vle8_v_u8m1(pixels + x, vl);
            __riscv_vse8_v_u8m1(block + x, v, vl);
            x += vl;
        }
        pixels += line_size;
        block += line_size;
    }
}
