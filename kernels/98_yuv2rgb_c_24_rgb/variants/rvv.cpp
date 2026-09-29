#include "kernel.h"

#include <riscv_vector.h>

// The upstream 24-bit YUV420 converter visits pairs of rows and only emits
// complete four-pixel groups. Gather each pixel's chroma, table pointer and
// final table byte independently; the three RGB components use strided stores.
int yuv2rgb_c_24_rgb_rvv(SwsInternal *c, const uint8_t *const src[],
                          const int srcStride[], int srcSliceY, int srcSliceH,
                          uint8_t *const dst[], const int dstStride[]) {
    const int width = c->opts.dst_w & ~3;
    for (int y = 0; y < srcSliceH; y += 2) {
        const uint8_t *u = src[1] + (y / 2) * srcStride[1];
        const uint8_t *v = src[2] + (y / 2) * srcStride[2];
        for (int row = 0; row < 2; ++row) {
            const uint8_t *py = src[0] + (y + row) * srcStride[0];
            uint8_t *out = dst[0] + (srcSliceY + y + row) * dstStride[0];
            for (int x = 0; x < width;) {
                const size_t vl = __riscv_vsetvl_e8m1(width - x);
                auto idx = __riscv_vadd_vx_u64m8(__riscv_vid_v_u64m8(vl), x, vl);
                idx = __riscv_vsrl_vx_u64m8(idx, 1, vl);
                const auto uv = __riscv_vluxei64_v_u8m1(u, idx, vl);
                const auto vv = __riscv_vluxei64_v_u8m1(v, idx, vl);
                const auto u64 = __riscv_vzext_vf8_u64m8(uv, vl);
                const auto v64 = __riscv_vzext_vf8_u64m8(vv, vl);
                const auto rindex = __riscv_vsll_vx_u64m8(
                    __riscv_vadd_vx_u64m8(v64, YUVRGB_TABLE_HEADROOM, vl), 3, vl);
                const auto bindex = __riscv_vsll_vx_u64m8(
                    __riscv_vadd_vx_u64m8(u64, YUVRGB_TABLE_HEADROOM, vl), 3, vl);
                const auto rp = __riscv_vluxei64_v_u64m8(
                    reinterpret_cast<const uint64_t *>(c->table_rV), rindex, vl);
                const auto bp = __riscv_vluxei64_v_u64m8(
                    reinterpret_cast<const uint64_t *>(c->table_bU), bindex, vl);
                const auto gp = __riscv_vluxei64_v_u64m8(
                    reinterpret_cast<const uint64_t *>(c->table_gU), bindex, vl);
                const auto gindex = __riscv_vsrl_vx_u64m8(rindex, 1, vl);
                const auto gv = __riscv_vluxei64_v_i32m4(c->table_gV, gindex, vl);
                const auto gptr = __riscv_vadd_vv_u64m8(
                    gp, __riscv_vreinterpret_v_i64m8_u64m8(
                        __riscv_vsext_vf2_i64m8(gv, vl)), vl);
                const auto yy = __riscv_vle8_v_u8m1(py + x, vl);
                const auto yi = __riscv_vzext_vf8_u64m8(yy, vl);
                const auto red = __riscv_vluxei64_v_u8m1(
                    static_cast<const uint8_t *>(nullptr), __riscv_vadd_vv_u64m8(rp, yi, vl), vl);
                const auto green = __riscv_vluxei64_v_u8m1(
                    static_cast<const uint8_t *>(nullptr), __riscv_vadd_vv_u64m8(gptr, yi, vl), vl);
                const auto blue = __riscv_vluxei64_v_u8m1(
                    static_cast<const uint8_t *>(nullptr), __riscv_vadd_vv_u64m8(bp, yi, vl), vl);
                __riscv_vsse8_v_u8m1(out + 3 * x, 3, red, vl);
                __riscv_vsse8_v_u8m1(out + 3 * x + 1, 3, green, vl);
                __riscv_vsse8_v_u8m1(out + 3 * x + 2, 3, blue, vl);
                x += static_cast<int>(vl);
            }
        }
    }
    return srcSliceH;
}
