#include "kernel.h"

#include <cassert>
#include <riscv_vector.h>

namespace {
// memcpy's source and destination for THIS row must not overlap. Other rows
// may alias: each call completes before any subsequent row is touched.
void copy_row(uint8_t *dst, const uint8_t *src, size_t bytes) {
    for (size_t i = 0; i < bytes;) {
        const size_t vl = __riscv_vsetvl_e8m1(bytes - i);
        const vuint8m1_t v = __riscv_vle8_v_u8m1(src + i, vl);
        __riscv_vse8_v_u8m1(dst + i, v, vl);
        i += vl;
    }
}

void fill_pixels(pixel *dst, int count, pixel value) {
    for (int i = 0; i < count;) {
        const size_t vl = __riscv_vsetvl_e16m1(count - i);
        const vuint16m1_t v = __riscv_vmv_v_x_u16m1(value, vl);
        __riscv_vse16_v_u16m1(dst + i, v, vl);
        i += static_cast<int>(vl);
    }
}
} // namespace

void emulated_edge_mc_16_rvv(uint8_t *buf, const uint8_t *src,
                         ptrdiff_t buf_linesize, ptrdiff_t src_linesize,
                         int block_w, int block_h, int src_x, int src_y,
                         int w, int h) {
    if (!w || !h)
        return;
    assert(static_cast<ptrdiff_t>(block_w * sizeof(pixel)) <=
           (buf_linesize < 0 ? -buf_linesize : buf_linesize));

    if (src_y >= h) {
        src -= src_y * src_linesize;
        src += (h - 1) * src_linesize;
        src_y = h - 1;
    } else if (src_y <= -block_h) {
        src -= src_y * src_linesize;
        src += (1 - block_h) * src_linesize;
        src_y = 1 - block_h;
    }
    if (src_x >= w) {
        src -= (1 + src_x - w) * sizeof(pixel);
        src_x = w - 1;
    } else if (src_x <= -block_w) {
        src += (1 - block_w - src_x) * sizeof(pixel);
        src_x = 1 - block_w;
    }

    const int start_y = src_y < 0 ? -src_y : 0;
    const int start_x = src_x < 0 ? -src_x : 0;
    const int end_y = block_h < h - src_y ? block_h : h - src_y;
    const int end_x = block_w < w - src_x ? block_w : w - src_x;
    assert(start_y < end_y && block_h);
    assert(start_x < end_x && block_w);

    const size_t bytes = (end_x - start_x) * sizeof(pixel);
    src += start_y * src_linesize + start_x * (ptrdiff_t)sizeof(pixel);
    buf += start_x * sizeof(pixel);

    int y = 0;
    for (; y < start_y; ++y) {
        copy_row(buf, src, bytes);
        buf += buf_linesize;
    }
    for (; y < end_y; ++y) {
        copy_row(buf, src, bytes);
        src += src_linesize;
        buf += buf_linesize;
    }
    src -= src_linesize;
    for (; y < block_h; ++y) {
        copy_row(buf, src, bytes);
        buf += buf_linesize;
    }

    buf -= block_h * buf_linesize + start_x * (ptrdiff_t)sizeof(pixel);
    while (block_h--) {
        pixel *bufp = reinterpret_cast<pixel *>(buf);
        // Read each edge pixel at the same point as the original left/right
        // loops: previous row's margin writes can affect this row's values.
        if (start_x)
            fill_pixels(bufp, start_x, bufp[start_x]);
        if (end_x < block_w)
            fill_pixels(bufp + end_x, block_w - end_x, bufp[end_x - 1]);
        buf += buf_linesize;
    }
}
