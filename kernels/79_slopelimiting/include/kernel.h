/****************************************************************************
 *
 *
 *  Project: Little CMS 2 (lcms2) 2.19.1
 *  Source files:
 *    include/lcms2.h
 *    include/lcms2_plugin.h
 *    src/lcms2_internal.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * include/lcms2.h
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
 *
 * include/lcms2_plugin.h
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
 *
 * src/lcms2_internal.h
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

#ifndef KERNELS_79_SLOPELIMITING_INCLUDE_KERNEL_H_
#define KERNELS_79_SLOPELIMITING_INCLUDE_KERNEL_H_

#include <assert.h>
#include <limits.h>
#include <math.h>

/*
 * include/lcms2.h:100-165
 */
#ifndef CMS_BASIC_TYPES_ALREADY_DEFINED

typedef unsigned char cmsUInt8Number;
typedef signed char cmsInt8Number;

#if CHAR_BIT != 8
#error "Unable to find 8 bit type, unsupported compiler"
#endif

typedef float cmsFloat32Number;
typedef double cmsFloat64Number;

#if (USHRT_MAX == 65535U)
typedef unsigned short cmsUInt16Number;
#elif (UINT_MAX == 65535U)
typedef unsigned int cmsUInt16Number;
#else
#error "Unable to find 16 bits unsigned type, unsupported compiler"
#endif

#if (SHRT_MAX == 32767)
typedef short cmsInt16Number;
#elif (INT_MAX == 32767)
typedef int cmsInt16Number;
#else
#error "Unable to find 16 bits signed type, unsupported compiler"
#endif

#if (UINT_MAX == 4294967295U)
typedef unsigned int cmsUInt32Number;
#elif (ULONG_MAX == 4294967295U)
typedef unsigned long cmsUInt32Number;
#else
#error "Unable to find 32 bit unsigned type, unsupported compiler"
#endif

#if (INT_MAX == +2147483647)
typedef int cmsInt32Number;
#elif (LONG_MAX == +2147483647)
typedef long cmsInt32Number;
#else
#error "Unable to find 32 bit signed type, unsupported compiler"
#endif

#ifndef CMS_DONT_USE_INT64
#if (ULONG_MAX == 18446744073709551615U)
typedef unsigned long cmsUInt64Number;
#elif (ULLONG_MAX == 18446744073709551615U)
typedef unsigned long long cmsUInt64Number;
#else
#define CMS_DONT_USE_INT64 1
#endif
#if (LONG_MAX == +9223372036854775807)
typedef long cmsInt64Number;
#elif (LLONG_MAX == +9223372036854775807)
typedef long long cmsInt64Number;
#else
#define CMS_DONT_USE_INT64 1
#endif
#endif

#endif

/*
 * include/lcms2.h:190
 */
typedef int cmsBool;

/*
 * include/lcms2.h:268-269 (GCC/Clang static build)
 */
#define CMSEXPORT
#define CMSAPI

/*
 * include/lcms2.h:1109
 */
typedef struct _cmsContext_struct *cmsContext;

/*
 * include/lcms2.h:1232-1239
 */
typedef struct {
    cmsFloat32Number x0, x1;
    cmsInt32Number Type;
    cmsFloat64Number Params[10];
    cmsUInt32Number nGridPoints;
    cmsFloat32Number *SampledPoints;

} cmsCurveSegment;

/*
 * include/lcms2.h:1242
 */
typedef struct _cms_curve_struct cmsToneCurve;

/*
 * include/lcms2.h:1261
 */
CMSAPI cmsBool CMSEXPORT cmsIsToneCurveDescending(const cmsToneCurve *t);

/*
 * include/lcms2_plugin.h:258
 */
struct _cms_interp_struc;

/*
 * include/lcms2_plugin.h:265-267 (C++ configuration)
 */
typedef void (*_cmsInterpFn16)(const cmsUInt16Number Input[],
                               cmsUInt16Number Output[],
                               const struct _cms_interp_struc *p);

/*
 * include/lcms2_plugin.h:272-274
 */
typedef void (*_cmsInterpFnFloat)(cmsFloat32Number const Input[],
                                  cmsFloat32Number Output[],
                                  const struct _cms_interp_struc *p);

/*
 * include/lcms2_plugin.h:279-282
 */
typedef union {
    _cmsInterpFn16 Lerp16;
    _cmsInterpFnFloat LerpFloat;
} cmsInterpFunction;

/*
 * include/lcms2_plugin.h:290
 */
#define MAX_INPUT_DIMENSIONS 15

/*
 * include/lcms2_plugin.h:292-310
 */
typedef struct _cms_interp_struc {
    cmsContext ContextID;

    cmsUInt32Number dwFlags;
    cmsUInt32Number nInputs;
    cmsUInt32Number nOutputs;

    cmsUInt32Number nSamples[MAX_INPUT_DIMENSIONS];
    cmsUInt32Number Domain[MAX_INPUT_DIMENSIONS];

    cmsUInt32Number opta[MAX_INPUT_DIMENSIONS];

    const void *Table;
    cmsInterpFunction Interpolation;

} cmsInterpParams;

/*
 * include/lcms2_plugin.h:329
 */
typedef cmsFloat64Number (*cmsParametricCurveEvaluator)(
    cmsInt32Number Type, const cmsFloat64Number Params[10], cmsFloat64Number R);

/*
 * src/lcms2_internal.h:87-88 (GCC/Clang configuration)
 */
#define cmsINLINE static inline

/*
 * src/lcms2_internal.h:135 (GCC/Clang configuration)
 */
#define _cmsAssert(a) assert((a))

/*
 * src/lcms2_internal.h:160-195 (little-endian fast-floor configuration)
 */
cmsINLINE int _cmsQuickFloor(cmsFloat64Number val) {
    const cmsFloat64Number _lcms_double2fixmagic = 68719476736.0 * 1.5;
    union {
        cmsFloat64Number val;
        int halves[2];
    } temp;

    temp.val = val + _lcms_double2fixmagic;

    return temp.halves[0] >> 16;
}

cmsINLINE cmsUInt16Number _cmsQuickFloorWord(cmsFloat64Number d) {
    return (cmsUInt16Number)_cmsQuickFloor(d - 32767.0) + 32767U;
}

cmsINLINE cmsUInt16Number _cmsQuickSaturateWord(cmsFloat64Number d) {
    d += 0.5;
    if (d <= 0)
        return 0;
    if (d >= 65535.0)
        return 0xffff;

    return _cmsQuickFloorWord(d);
}

/*
 * src/lcms2_internal.h:880-893
 */
struct _cms_curve_struct {

    cmsInterpParams *InterpParams;

    cmsUInt32Number nSegments;
    cmsCurveSegment *Segments;
    cmsInterpParams **SegInterp;

    cmsParametricCurveEvaluator *Evals;

    cmsUInt32Number nEntries;
    cmsUInt16Number *Table16;
};

/*
 * Wrapper for invoking the extracted kernel.
 */
void SlopeLimiting_isolated(cmsToneCurve *g);

#endif // KERNELS_79_SLOPELIMITING_INCLUDE_KERNEL_H_
