/****************************************************************************
 *
 *
 *  Project: FreeType 2.14.3
 *  Source files:
 *    include/freetype/config/integer-types.h
 *    include/freetype/config/public-macros.h
 *    include/freetype/fttypes.h
 *    include/freetype/ftimage.h
 *    include/freetype/freetype.h
 *    include/freetype/ftoutln.h
 *    include/freetype/fterrors.h
 *    include/freetype/fterrdef.h
 *    include/freetype/internal/compiler-macros.h
 *    include/freetype/internal/ftdebug.h
 *    include/freetype/internal/ftcalc.h
 *    include/freetype/internal/ftobjs.h
 *    include/freetype/ftbbox.h
 *
 *
 *  Below are the copyright notices of original files
 *
 *
 * include/freetype/config/integer-types.h
 *
 *   FreeType integer types definitions.
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/config/public-macros.h
 *
 *   Define a set of compiler macros used in public FreeType headers.
 *
 * Copyright (C) 2020-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/fttypes.h
 *
 *   FreeType simple types definitions (specification only).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/ftimage.h
 *
 *   FreeType glyph image formats and default raster interface
 *   (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/freetype.h
 *
 *   FreeType high-level API and common types (specification only).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/ftoutln.h
 *
 *   Support for the FT_Outline type used to store glyph shapes of
 *   most scalable font formats (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/fterrors.h
 *
 *   FreeType error code handling (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/fterrdef.h
 *
 *   FreeType error codes (specification).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/internal/compiler-macros.h
 *
 *   Compiler-specific macro definitions used internally by FreeType.
 *
 * Copyright (C) 2020-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/internal/ftdebug.h
 *
 *   Debugging and logging component (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * IMPORTANT: A description of FreeType's debugging support can be
 *             found in 'docs/DEBUG.TXT'.  Read it if you need to use or
 *             understand this code.
 *
 *
 * include/freetype/internal/ftcalc.h
 *
 *   Arithmetic computations (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 *
 * include/freetype/internal/ftobjs.h
 *
 *   The FreeType private base classes (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 * include/freetype/ftbbox.h
 *
 *   FreeType exact bbox computation (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

#ifndef KERNELS_27_FT_OUTLINE_GET_BBOX_INCLUDE_KERNEL_H_
#define KERNELS_27_FT_OUTLINE_GET_BBOX_INCLUDE_KERNEL_H_

extern "C" {

/*
 * include/freetype/config/integer-types.h:165 (32-bit int)
 */
typedef unsigned int FT_UInt32;

/*
 * include/freetype/config/integer-types.h:195 (64-bit long)
 */
#define FT_UINT64 unsigned long

/*
 * include/freetype/config/integer-types.h:249 (64-bit long selected)
 */
typedef FT_UINT64 FT_UInt64;

/*
 * include/freetype/fttypes.h:157, :212, :223, :234, :245, :256 and :302
 */
typedef unsigned char FT_Byte;
typedef unsigned short FT_UShort;
typedef signed int FT_Int;
typedef unsigned int FT_UInt;
typedef signed long FT_Long;
typedef unsigned long FT_ULong;
typedef int FT_Error;

/*
 * include/freetype/ftimage.h:57
 */
typedef signed long FT_Pos;

/*
 * include/freetype/ftimage.h:75-80
 */
typedef struct FT_Vector_ {
    FT_Pos x;
    FT_Pos y;

} FT_Vector;

/*
 * include/freetype/ftimage.h:118-123
 */
typedef struct FT_BBox_ {
    FT_Pos xMin, yMin;
    FT_Pos xMax, yMax;

} FT_BBox;

/*
 * include/freetype/ftimage.h:351-362
 */
typedef struct FT_Outline_ {
    unsigned short n_contours;
    unsigned short n_points;

    FT_Vector *points;
    unsigned char *tags;
    unsigned short *contours;

    int flags;

} FT_Outline;

/*
 * include/freetype/ftimage.h:474-479
 */
#define FT_CURVE_TAG(flag) (flag & 0x03)

#define FT_CURVE_TAG_ON 0x01
#define FT_CURVE_TAG_CONIC 0x00
#define FT_CURVE_TAG_CUBIC 0x02

/*
 * include/freetype/ftimage.h:522-526
 */
typedef int (*FT_Outline_MoveToFunc)(const FT_Vector *to, void *user);

#define FT_Outline_MoveTo_Func FT_Outline_MoveToFunc

/*
 * include/freetype/ftimage.h:551-555
 */
typedef int (*FT_Outline_LineToFunc)(const FT_Vector *to, void *user);

#define FT_Outline_LineTo_Func FT_Outline_LineToFunc

/*
 * include/freetype/ftimage.h:585-590
 */
typedef int (*FT_Outline_ConicToFunc)(const FT_Vector *control,
                                      const FT_Vector *to, void *user);

#define FT_Outline_ConicTo_Func FT_Outline_ConicToFunc

/*
 * include/freetype/ftimage.h:621-627
 */
typedef int (*FT_Outline_CubicToFunc)(const FT_Vector *control1,
                                      const FT_Vector *control2,
                                      const FT_Vector *to, void *user);

#define FT_Outline_CubicTo_Func FT_Outline_CubicToFunc

/*
 * include/freetype/ftimage.h:673-683
 */
typedef struct FT_Outline_Funcs_ {
    FT_Outline_MoveToFunc move_to;
    FT_Outline_LineToFunc line_to;
    FT_Outline_ConicToFunc conic_to;
    FT_Outline_CubicToFunc cubic_to;

    int shift;
    FT_Pos delta;

} FT_Outline_Funcs;

/*
 * include/freetype/internal/ftcalc.h:54-55
 */
#define NEG_LONG(a) (FT_Long)((FT_ULong)0 - (FT_ULong)(a))

/*
 * include/freetype/internal/ftcalc.h:331-338 (GCC/Clang, 32-bit int)
 */
#define FT_MSB(x) (31 - __builtin_clz(x))

/*
 * include/freetype/config/public-macros.h:75-77 (GCC/Clang)
 */
#define FT_PUBLIC_FUNCTION_ATTRIBUTE __attribute__((visibility("default")))

/*
 * include/freetype/config/public-macros.h:104
 */
#define FT_EXPORT(x) FT_PUBLIC_FUNCTION_ATTRIBUTE extern x

/*
 * include/freetype/internal/compiler-macros.h:69-70
 */
#define FT_BEGIN_STMNT do {
#define FT_END_STMNT                                                           \
    }                                                                          \
    while (0)

/*
 * include/freetype/internal/compiler-macros.h:188-190 (C++ selection)
 */
#define FT_FUNCTION_DEFINITION(x) extern "C" x

/*
 * include/freetype/internal/compiler-macros.h:278
 */
#define FT_EXPORT_DEF(x) FT_FUNCTION_DEFINITION(x)

/*
 * include/freetype/internal/compiler-macros.h:312-314 (C++ selection)
 */
#define FT_CALLBACK_DEF(x) extern "C" x

/*
 * include/freetype/internal/ftdebug.h:153-165
 */
#define FT_TRACE(level, varformat)                                             \
    do {                                                                       \
    } while (0)

/*
 * include/freetype/internal/ftdebug.h:253
 */
#define FT_TRACE5(varformat) FT_TRACE(5, varformat)

/*
 * include/freetype/internal/ftdebug.h:329 (non-debug selection)
 */
#define FT_THROW(e) FT_ERR_CAT(FT_ERR_PREFIX, e)

/*
 * include/freetype/fttypes.h:596-597
 */
#define FT_ERR_XCAT(x, y) x##y
#define FT_ERR_CAT(x, y) FT_ERR_XCAT(x, y)

/*
 * include/freetype/fterrors.h:145-147 and :152-163
 */
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX FT_Err_
#endif

#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS

#ifndef FT_ERR_BASE
#define FT_ERR_BASE FT_Mod_Err_Base
#endif

#else

#undef FT_ERR_BASE
#define FT_ERR_BASE 0

#endif

/*
 * include/freetype/fterrors.h:169-182 and :186-191
 */
#ifndef FT_ERRORDEF

#define FT_INCLUDE_ERR_PROTOS

#define FT_ERRORDEF(e, v, s) e = v,
#define FT_ERROR_START_LIST enum {
#define FT_ERROR_END_LIST                                                      \
    FT_ERR_CAT(FT_ERR_PREFIX, Max)                                             \
    }                                                                          \
    ;

#ifdef __cplusplus
#define FT_NEED_EXTERN_C
extern "C" {
#endif

#endif

#define FT_ERRORDEF_(e, v, s)                                                  \
    FT_ERRORDEF(FT_ERR_CAT(FT_ERR_PREFIX, e), v + FT_ERR_BASE, s)

#define FT_NOERRORDEF_(e, v, s) FT_ERRORDEF(FT_ERR_CAT(FT_ERR_PREFIX, e), v, s)

/*
 * include/freetype/fterrors.h:194-196 and :203-205 (the
 * FT_ERROR_START_LIST .. FT_ERROR_END_LIST instantiation) and
 * include/freetype/fterrdef.h:58-59, :71-72 and :96-97 (the copied entries;
 * the <freetype/fterrdef.h> include at fterrors.h:200 is replaced by them)
 */
FT_ERROR_START_LIST
FT_NOERRORDEF_(Ok, 0x00, "no error")

FT_ERRORDEF_(Invalid_Argument, 0x06, "invalid argument")

FT_ERRORDEF_(Invalid_Outline, 0x14, "invalid outline")

FT_ERROR_END_LIST

/*
 * include/freetype/fterrors.h:216-228
 */
#ifdef FT_NEED_EXTERN_C
}
#endif

#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST

#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_

#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE

/*
 * include/freetype/internal/ftobjs.h:73
 */
#define FT_ABS(a) ((a) < 0 ? -(a) : (a))

/*
 * include/freetype/internal/ftobjs.h:993-1009
 */
#define FT_DEFINE_OUTLINE_FUNCS(class_, move_to_, line_to_, conic_to_,         \
                                cubic_to_, shift_, delta_)                     \
    static const FT_Outline_Funcs class_ = {move_to_,  line_to_, conic_to_,    \
                                            cubic_to_, shift_,   delta_};

/*
 * include/freetype/freetype.h:4988-4992
 */
FT_EXPORT(FT_Long)
FT_MulDiv(FT_Long a, FT_Long b, FT_Long c);

/*
 * include/freetype/ftoutln.h:124-127
 */
FT_EXPORT(FT_Error)
FT_Outline_Decompose(FT_Outline *outline,
                     const FT_Outline_Funcs *func_interface, void *user);

/*
 * include/freetype/ftbbox.h:84-86
 */
FT_EXPORT(FT_Error)
FT_Outline_Get_BBox(FT_Outline *outline, FT_BBox *abbox);

} // extern "C"
#endif // KERNELS_27_FT_OUTLINE_GET_BBOX_INCLUDE_KERNEL_H_
