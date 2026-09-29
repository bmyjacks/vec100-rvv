#include "kernel.h"

// Pinned x265 3.4 source/common/loopfilter.cpp:39-43, 80-97, with only
// the local pixel/clip definitions and external entry point substituted.
static inline int8_t signOf(int x) {
    return (x >> 31) | ((int)((((uint32_t)-x)) >> 31));
}

static inline pixel x265_clip(int v) {
    return static_cast<pixel>(v < 0 ? 0 : v > 255 ? 255 : v);
}

void process_sao_cue1_2rows(pixel *rec, int8_t *upBuff1, int8_t *offsetEo,
                             intptr_t stride, int width) {
    int x, y;
    int8_t signDown;
    int edgeType;

    for (y = 0; y < 2; y++) {
        for (x = 0; x < width; x++) {
            signDown = signOf(rec[x] - rec[x + stride]);
            edgeType = signDown + upBuff1[x] + 2;
            upBuff1[x] = -signDown;
            rec[x] = x265_clip(rec[x] + offsetEo[edgeType]);
        }
        rec += stride;
    }
}
