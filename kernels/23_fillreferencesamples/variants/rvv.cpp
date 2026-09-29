#include "kernel.h"
#include <riscv_vector.h>

static void fill(pixel *p, size_t count, pixel value) {
    for (size_t i = 0; i < count;) {
        size_t vl = __riscv_vsetvl_e8m1(count - i);
        __riscv_vse8_v_u8m1(p + i, __riscv_vmv_v_x_u8m1(value, vl), vl);
        i += vl;
    }
}

static void copy(pixel *out, const pixel *in, size_t count) {
    for (size_t i = 0; i < count;) {
        size_t vl = __riscv_vsetvl_e8m1(count - i);
        __riscv_vse8_v_u8m1(out + i, __riscv_vle8_v_u8m1(in + i, vl), vl);
        i += vl;
    }
}

void fillReferenceSamples_rvv(const pixel *origin, intptr_t stride,
                              const Predict::IntraNeighbors &nb, pixel dst[258]) {
    const pixel dc = (pixel)(1 << (X265_DEPTH - 1));
    const unsigned ref = (2u << nb.log2TrSize) + 1;
    if (nb.numIntraNeighbor == 0) {
        fill(dst, 2 * ref - 1, dc);
        return;
    }
    if (nb.numIntraNeighbor == nb.totalUnits) {
        copy(dst, origin - stride - 1, ref);
        for (size_t i = 0; i < ref - 1;) {
            size_t vl = __riscv_vsetvl_e8m1(ref - 1 - i);
            auto v = __riscv_vlse8_v_u8m1(origin - 1 + i * stride, stride, vl);
            __riscv_vse8_v_u8m1(dst + ref + i, v, vl);
            i += vl;
        }
        return;
    }

    const int left = nb.leftUnits, width = nb.unitWidth, height = nb.unitHeight;
    const int total = left * height + (nb.aboveUnits + 1) * width;
    pixel line[5 * MAX_CU_SIZE];
    fill(line, total, dc);
    if (nb.bNeighborFlags[left])
        fill(line + left * height, width, origin[-stride - 1]);

    // The upstream partial-availability path deliberately copies unavailable
    // samples too; propagation below subsequently repairs their units.
    for (int j = 0; j < left * height;) {
        size_t vl = __riscv_vsetvl_e8m1(left * height - j);
        auto v = __riscv_vlse8_v_u8m1(origin - 1 + j * stride, stride, vl);
        __riscv_vsse8_v_u8m1(line + left * height - 1 - j, -1, v, vl);
        j += vl;
    }
    copy(line + left * height + width, origin - stride,
         nb.aboveUnits * width);

    int curr = 0, next = 1;
    pixel *adi = line;
    if (!nb.bNeighborFlags[0]) {
        while (next < nb.totalUnits && !nb.bNeighborFlags[next]) ++next;
        const int offset = left * (height - width);
        pixel sample = line[next < left ? next * height : offset + next * width];
        int top = X265_MIN(next, left);
        if (curr < top) {
            int count = height * (top - curr);
            fill(adi, count, sample);
            curr = top; adi += count;
        }
        if (curr < next) {
            int count = width * (next - curr);
            fill(adi, count, sample);
            curr = next; adi += count;
        }
    }
    // Sequential unit dependency: an absent unit inherits the last sample
    // of the preceding (possibly also absent) unit.
    while (curr < nb.totalUnits) {
        const int count = curr >= left ? width : height;
        if (!nb.bNeighborFlags[curr]) fill(adi, count, adi[-1]);
        adi += count;
        ++curr;
    }
    copy(dst, line + ref + width - 2, ref);
    for (size_t i = 0; i < ref - 1;) {
        size_t vl = __riscv_vsetvl_e8m1(ref - 1 - i);
        auto v = __riscv_vlse8_v_u8m1(line + ref - 2 - i, -1, vl);
        __riscv_vse8_v_u8m1(dst + ref + i, v, vl);
        i += vl;
    }
}
