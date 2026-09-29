/****************************************************************************
 *
 *
 *  Project: FreeType 2.14.3
 *  Source files:
 *    include/freetype/config/public-macros.h
 *    include/freetype/config/ftstdlib.h
 *    include/freetype/fttypes.h
 *    include/freetype/ftimage.h
 *    include/freetype/ftsystem.h
 *    include/freetype/freetype.h
 *    include/freetype/ftbitmap.h
 *    include/freetype/fterrors.h
 *    include/freetype/fterrdef.h
 *    include/freetype/internal/compiler-macros.h
 *    include/freetype/internal/ftdebug.h
 *    include/freetype/internal/ftobjs.h
 *    include/freetype/internal/ftmemory.h
 *
 *
 *  Below are the copyright notices of original files
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
 * include/freetype/config/ftstdlib.h
 *
 *   ANSI-specific library and header configuration file (specification
 *   only).
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
 * include/freetype/ftsystem.h
 *
 *   FreeType low-level system interface definition (specification).
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
 * include/freetype/ftbitmap.h
 *
 *   FreeType utility functions for bitmaps (specification).
 *
 * Copyright (C) 2004-2026 by
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
 *
 * include/freetype/internal/ftmemory.h
 *
 *   The FreeType memory management macros (specification).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

#ifndef KERNELS_26_FT_BITMAP_EMBOLDEN_INCLUDE_KERNEL_H_
#define KERNELS_26_FT_BITMAP_EMBOLDEN_INCLUDE_KERNEL_H_

#include <limits.h>
#include <stddef.h>
#include <string.h>

/*
 * include/freetype/fttypes.h:157, :223, :234, :245, :302, :313 and :326
 */
typedef unsigned char FT_Byte;
typedef signed int FT_Int;
typedef unsigned int FT_UInt;
typedef signed long FT_Long;
typedef int FT_Error;
typedef void *FT_Pointer;
typedef size_t FT_Offset;

/*
 * include/freetype/fttypes.h:596-597
 */
#define FT_ERR_XCAT(x, y) x##y
#define FT_ERR_CAT(x, y) FT_ERR_XCAT(x, y)

/*
 * include/freetype/ftimage.h:57
 */
typedef signed long FT_Pos;

/*
 * include/freetype/ftimage.h:180-193
 */
typedef enum FT_Pixel_Mode_ {
    FT_PIXEL_MODE_NONE = 0,
    FT_PIXEL_MODE_MONO,
    FT_PIXEL_MODE_GRAY,
    FT_PIXEL_MODE_GRAY2,
    FT_PIXEL_MODE_GRAY4,
    FT_PIXEL_MODE_LCD,
    FT_PIXEL_MODE_LCD_V,
    FT_PIXEL_MODE_BGRA,

    FT_PIXEL_MODE_MAX

} FT_Pixel_Mode;

/*
 * include/freetype/ftimage.h:275-286
 */
typedef struct FT_Bitmap_ {
    unsigned int rows;
    unsigned int width;
    int pitch;
    unsigned char *buffer;
    unsigned short num_grays;
    unsigned char pixel_mode;
    unsigned char palette_mode;
    void *palette;

} FT_Bitmap;

/*
 * include/freetype/ftsystem.h:64
 */
typedef struct FT_MemoryRec_ *FT_Memory;

/*
 * include/freetype/ftsystem.h:86-88
 */
typedef void *(*FT_Alloc_Func)(FT_Memory memory, long size);

/*
 * include/freetype/ftsystem.h:107-109
 */
typedef void (*FT_Free_Func)(FT_Memory memory, void *block);

/*
 * include/freetype/ftsystem.h:140-144
 */
typedef void *(*FT_Realloc_Func)(FT_Memory memory, long cur_size, long new_size,
                                 void *block);

/*
 * include/freetype/ftsystem.h:169-175
 */
struct FT_MemoryRec_ {
    void *user;
    FT_Alloc_Func alloc;
    FT_Free_Func free;
    FT_Realloc_Func realloc;
};

/*
 * include/freetype/freetype.h:557
 */
typedef struct FT_LibraryRec_ *FT_Library;

/*
 * include/freetype/internal/ftobjs.h:896-898 (partial projection of
 * FT_LibraryRec_; the copied functions only use its `memory' member, and no
 * other member is referenced)
 */
typedef struct FT_LibraryRec_ {
    FT_Memory memory;
} FT_LibraryRec;

/*
 * include/freetype/config/ftstdlib.h:64, :92 and :94
 */
#define FT_INT_MAX INT_MAX
#define ft_memcpy memcpy
#define ft_memset memset

/*
 * include/freetype/internal/compiler-macros.h:69-70
 */
#define FT_BEGIN_STMNT do {
#define FT_END_STMNT                                                           \
    }                                                                          \
    while (0)

/*
 * include/freetype/internal/compiler-macros.h:115-122
 */
#define FT_TYPEOF(type) (__typeof__(type))

/*
 * include/freetype/internal/compiler-macros.h:132-146
 */
#define FT_INTERNAL_FUNCTION_ATTRIBUTE __attribute__((visibility("hidden")))

/*
 * include/freetype/internal/compiler-macros.h:180, :188-192, :228-230 and
 * :278
 */
#define FT_FUNCTION_DECLARATION(x) extern x

#define FT_FUNCTION_DEFINITION(x) extern "C" x

#define FT_BASE(x)                                                             \
    FT_INTERNAL_FUNCTION_ATTRIBUTE                                             \
    FT_FUNCTION_DECLARATION(x)
#define FT_BASE_DEF(x) FT_FUNCTION_DEFINITION(x)

#define FT_EXPORT_DEF(x) FT_FUNCTION_DEFINITION(x)

/*
 * include/freetype/config/public-macros.h:66-87
 */
#define FT_PUBLIC_FUNCTION_ATTRIBUTE __attribute__((visibility("default")))

/*
 * include/freetype/config/public-macros.h:104
 */
#define FT_EXPORT(x) FT_PUBLIC_FUNCTION_ATTRIBUTE extern x

/*
 * include/freetype/internal/ftdebug.h:309-331
 */
#define FT_ASSERT(condition)                                                   \
    do {                                                                       \
    } while (0)

#define FT_THROW(e) FT_ERR_CAT(FT_ERR_PREFIX, e)

/*
 * include/freetype/fterrors.h:145-147 and :152-163
 */
#define FT_ERR_PREFIX FT_Err_
#define FT_ERR_BASE 0

/*
 * include/freetype/fterrors.h:169-182 and :186-191
 */
#define FT_ERRORDEF(e, v, s) e = v,
#define FT_ERROR_START_LIST enum {
#define FT_ERROR_END_LIST                                                      \
    FT_ERR_CAT(FT_ERR_PREFIX, Max)                                             \
    }                                                                          \
    ;

extern "C" {

#define FT_ERRORDEF_(e, v, s)                                                  \
    FT_ERRORDEF(FT_ERR_CAT(FT_ERR_PREFIX, e), v + FT_ERR_BASE, s)

#define FT_NOERRORDEF_(e, v, s) FT_ERRORDEF(FT_ERR_CAT(FT_ERR_PREFIX, e), v, s)

/*
 * include/freetype/fterrors.h:194-196 and :203-205 (the
 * FT_ERROR_START_LIST .. FT_ERROR_END_LIST instantiation) and
 * include/freetype/fterrdef.h:58-59, :71-72, :79-80, :92-93, :111-112 and
 * :137-138 (the copied entries; the <freetype/fterrdef.h> include at
 * fterrors.h:200 is replaced by them)
 */
FT_ERROR_START_LIST
FT_NOERRORDEF_(Ok, 0x00, "no error")

FT_ERRORDEF_(Invalid_Argument, 0x06, "invalid argument")

FT_ERRORDEF_(Array_Too_Large, 0x0A, "array allocation size too large")

FT_ERRORDEF_(Invalid_Glyph_Format, 0x12, "unsupported glyph image format")

FT_ERRORDEF_(Invalid_Library_Handle, 0x21, "invalid library handle")

FT_ERRORDEF_(Out_Of_Memory, 0x40, "out of memory")

FT_ERROR_END_LIST

/*
 * include/freetype/fterrors.h:216-228
 */
}

#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST

#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_

#undef FT_ERR_BASE

/*
 * include/freetype/internal/ftobjs.h:73 and :91-92
 */
#define FT_ABS(a) ((a) < 0 ? -(a) : (a))

#define FT_PIX_FLOOR(x) ((x) & ~FT_TYPEOF(x) 63)
#define FT_PIX_ROUND(x) FT_PIX_FLOOR((x) + 32)

/*
 * include/freetype/internal/ftmemory.h:75-93
 */
extern "C++" {
template <typename T> inline T *cplusplus_typeof(T *, void *v) {
    return static_cast<T *>(v);
}
}

#define FT_ASSIGNP(p, val) (p) = cplusplus_typeof((p), (val))

/*
 * include/freetype/internal/ftmemory.h:97-115
 */
#define FT_DEBUG_INNER(exp) (exp)
#define FT_ASSIGNP_INNER(p, exp) FT_ASSIGNP(p, exp)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * include/freetype/internal/ftmemory.h:144-150 and :152-154
 */
FT_BASE(FT_Pointer)
ft_mem_qrealloc(FT_Memory memory, FT_Long item_size, FT_Long cur_count,
                FT_Long new_count, void *block, FT_Error *p_error);

FT_BASE(void)
ft_mem_free(FT_Memory memory, const void *P);

/*
 * include/freetype/internal/ftmemory.h:165-169
 */
#define FT_MEM_FREE(ptr)                                                       \
    FT_BEGIN_STMNT                                                             \
    FT_DEBUG_INNER(ft_mem_free(memory, (ptr)));                                \
    (ptr) = NULL;                                                              \
    FT_END_STMNT

/*
 * include/freetype/internal/ftmemory.h:214-220
 */
#define FT_MEM_QALLOC_MULT(ptr, count, item_size)                              \
    FT_ASSIGNP_INNER(ptr, ft_mem_qrealloc(memory, (FT_Long)(item_size), 0,     \
                                          (FT_Long)(count), NULL, &error))

/*
 * include/freetype/internal/ftmemory.h:231, :234-235, :237-239, :244 and
 * :253-256
 */
#define FT_MEM_SET_ERROR(cond) ((cond), error != 0)

#define FT_MEM_SET(dest, byte, count) ft_memset(dest, byte, (FT_Offset)(count))

#define FT_MEM_COPY(dest, source, count)                                       \
    ft_memcpy(dest, source, (FT_Offset)(count))

#define FT_MEM_ZERO(dest, count) FT_MEM_SET(dest, 0, count)

#define FT_ARRAY_COPY(dest, source, count)                                     \
    FT_MEM_COPY(dest, source, (FT_Offset)(count) * sizeof(*(dest)))

/*
 * include/freetype/internal/ftmemory.h:330-331 and :337
 */
#define FT_QALLOC_MULT(ptr, count, item_size)                                  \
    FT_MEM_SET_ERROR(FT_MEM_QALLOC_MULT(ptr, count, item_size))

#define FT_FREE(ptr) FT_MEM_FREE(ptr)

/*
 * include/freetype/ftbitmap.h:76-77
 */
FT_EXPORT(void)
FT_Bitmap_Init(FT_Bitmap *abitmap);

/*
 * include/freetype/ftbitmap.h:156-160
 */
FT_EXPORT(FT_Error)
FT_Bitmap_Embolden(FT_Library library, FT_Bitmap *bitmap, FT_Pos xStrength,
                   FT_Pos yStrength);

/*
 * include/freetype/ftbitmap.h:203-207
 */
FT_EXPORT(FT_Error)
FT_Bitmap_Convert(FT_Library library, const FT_Bitmap *source,
                  FT_Bitmap *target, FT_Int alignment);

/*
 * include/freetype/ftbitmap.h:316-318
 */
FT_EXPORT(FT_Error)
FT_Bitmap_Done(FT_Library library, FT_Bitmap *bitmap);

#ifdef __cplusplus
}
#endif

#endif // KERNELS_26_FT_BITMAP_EMBOLDEN_INCLUDE_KERNEL_H_
