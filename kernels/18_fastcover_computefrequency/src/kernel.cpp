/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/dictBuilder/fastcover.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * lib/dictBuilder/fastcover.c
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 */

#include "kernel.h"

/*
 * lib/dictBuilder/fastcover.c:103-106
 */
typedef struct {
    unsigned finalize;
    unsigned skip;
} FASTCOVER_accel_t;

/*
 * lib/dictBuilder/fastcover.c:127-139
 */
typedef struct {
    const BYTE *samples;
    size_t *offsets;
    const size_t *samplesSizes;
    size_t nbSamples;
    size_t nbTrainSamples;
    size_t nbTestSamples;
    size_t nbDmers;
    U32 *freqs;
    unsigned d;
    unsigned f;
    FASTCOVER_accel_t accelParams;
} FASTCOVER_ctx_t;

/*
 * lib/dictBuilder/fastcover.c:92-97
 */
static size_t FASTCOVER_hashPtrToIndex(const void *p, U32 f, unsigned d) {
    if (d == 6) {
        return ZSTD_hash6Ptr(p, f);
    }
    return ZSTD_hash8Ptr(p, f);
}

/*
 * lib/dictBuilder/fastcover.c:283-302
 */
static void FASTCOVER_computeFrequency(U32 *freqs, const FASTCOVER_ctx_t *ctx) {
    const unsigned f = ctx->f;
    const unsigned d = ctx->d;
    const unsigned skip = ctx->accelParams.skip;
    const unsigned readLength = MAX(d, 8);
    size_t i;
    assert(ctx->nbTrainSamples >= 5);
    assert(ctx->nbTrainSamples <= ctx->nbSamples);
    for (i = 0; i < ctx->nbTrainSamples; i++) {
        size_t start = ctx->offsets[i];
        size_t const currSampleEnd = ctx->offsets[i + 1];
        while (start + readLength <= currSampleEnd) {
            const size_t dmerIndex =
                FASTCOVER_hashPtrToIndex(ctx->samples + start, f, d);
            freqs[dmerIndex]++;
            start = start + skip + 1;
        }
    }
}

/*
 * Wrapper for invoking the extracted kernel: fills the caller-owned
 * FASTCOVER_ctx_t fields read by the copied static upstream function and
 * forwards to it. The upstream caller was FASTCOVER_ctx_init
 * (fastcover.c:390).
 */
void FASTCOVER_computeFrequency_bench_isolated(U32 *freqs, const BYTE *samples,
                                      size_t *offsets, size_t nbTrainSamples,
                                      size_t nbSamples, unsigned f, unsigned d,
                                      unsigned skip) {
    FASTCOVER_ctx_t ctx;
    ctx.samples = samples;
    ctx.offsets = offsets;
    ctx.samplesSizes = NULL;
    ctx.nbSamples = nbSamples;
    ctx.nbTrainSamples = nbTrainSamples;
    ctx.nbTestSamples = 0;
    ctx.nbDmers = 0;
    ctx.freqs = freqs;
    ctx.d = d;
    ctx.f = f;
    ctx.accelParams.finalize = 0;
    ctx.accelParams.skip = skip;
    FASTCOVER_computeFrequency(freqs, &ctx);
}
