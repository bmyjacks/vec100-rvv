#include "kernel.h"
#include <riscv_vector.h>

extern "C" FT_Error FT_Bitmap_Embolden_rvv(FT_Library library, FT_Bitmap *bitmap,
                                             FT_Pos xStrength, FT_Pos yStrength) {
    // The 1-pixel GRAY / zero-vertical-strength case needs no reallocation
    // when a row already has a spare byte.  This is exactly the no-allocate
    // branch of ft_bitmap_assure_buffer.  In all other cases retain FreeType's
    // allocator, conversion, vertical propagation, and multi-pixel recurrence.
    if (!library || !bitmap || !bitmap->buffer ||
        bitmap->pixel_mode != FT_PIXEL_MODE_GRAY || bitmap->num_grays != 256 ||
        xStrength < 32 || xStrength >= 96 ||
        yStrength < -31 || yStrength >= 32 ||
        !bitmap->rows || bitmap->width >= (unsigned)INT_MAX ||
        bitmap->pitch == INT_MIN ||
        (unsigned)FT_ABS(bitmap->pitch) > UINT_MAX / 8 ||
        (unsigned)FT_ABS(bitmap->pitch) <= bitmap->width)
        return FT_Bitmap_Embolden(library, bitmap, xStrength, yStrength);

    const int pitch = FT_ABS(bitmap->pitch);
    // The upstream assure_buffer clears every byte past the new visible width
    // before processing.  Its buffer pointer is the first physical row even
    // for negative pitch; the sweep itself follows the signed row stride.
    for (unsigned row = 0; row < bitmap->rows; ++row)
        memset(bitmap->buffer + (size_t)row * pitch + bitmap->width + 1,
               0, pitch - bitmap->width - 1);

    unsigned char *p = bitmap->pitch > 0 ? bitmap->buffer :
                       bitmap->buffer + (size_t)pitch * (bitmap->rows - 1);
    for (unsigned row = 0; row < bitmap->rows; ++row, p += bitmap->pitch) {
        // Sweep right-to-left: x-1 has not been written yet.  Fetch both
        // overlapping ranges before any store to preserve that dependency.
        for (int x = pitch - 1; x > 0;) {
            size_t vl = __riscv_vsetvl_e8m1(x);
            int begin = x - (int)vl + 1;
            auto cur = __riscv_vle8_v_u8m1(p + begin, vl);
            auto prev = __riscv_vle8_v_u8m1(p + begin - 1, vl);
            auto sum = __riscv_vsaddu_vv_u8m1(cur, prev, vl);
            __riscv_vse8_v_u8m1(p + begin, sum, vl);
            x -= vl;
        }
    }
    ++bitmap->width;
    return FT_Err_Ok;
}
