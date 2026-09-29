/****************************************************************************
 *
 *
 *  Project: Little CMS 2 (lcms2) 2.19.1
 *  Source files:
 *    src/cmsgamma.c
 *    src/cmsopt.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/cmsgamma.c
 *
 *   Little Color Management System
 *
 * Copyright (c) 1998-2026 Marti Maria Saguer
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * src/cmsopt.c
 *
 *   Little Color Management System
 *
 * Copyright (c) 1998-2026 Marti Maria Saguer
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "kernel.h"

/*
 * src/cmsgamma.c:1393-1398
 */
cmsBool CMSEXPORT cmsIsToneCurveDescending(const cmsToneCurve *t) {
    _cmsAssert(t != NULL);

    return t->Table16[0] > t->Table16[t->nEntries - 1];
}

/*
 * src/cmsopt.c:826-857
 */
static void SlopeLimiting(cmsToneCurve *g) {
    int BeginVal, EndVal;
    int AtBegin = (int)floor((cmsFloat64Number)g->nEntries * 0.02 + 0.5);
    int AtEnd = (int)g->nEntries - AtBegin - 1;
    cmsFloat64Number Val, Slope, beta;
    int i;

    if (cmsIsToneCurveDescending(g)) {
        BeginVal = 0xffff;
        EndVal = 0;
    } else {
        BeginVal = 0;
        EndVal = 0xffff;
    }

    Val = g->Table16[AtBegin];
    Slope = (Val - BeginVal) / AtBegin;
    beta = Val - Slope * AtBegin;

    for (i = 0; i < AtBegin; i++)
        g->Table16[i] = _cmsQuickSaturateWord(i * Slope + beta);

    Val = g->Table16[AtEnd];
    Slope = (EndVal - Val) / AtBegin;
    beta = Val - Slope * AtEnd;

    for (i = AtEnd; i < (int)g->nEntries; i++)
        g->Table16[i] = _cmsQuickSaturateWord(i * Slope + beta);
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void SlopeLimiting_isolated(cmsToneCurve *g) { SlopeLimiting(g); }
