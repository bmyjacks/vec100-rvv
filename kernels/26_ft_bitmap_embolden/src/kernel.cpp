/****************************************************************************
 *
 *
 *  Project: FreeType 2.14.3
 *  Source files:
 *    src/base/ftbitmap.c
 *    src/base/ftutil.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/base/ftbitmap.c
 *
 *   FreeType utility functions for bitmaps (body).
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
 * src/base/ftutil.c
 *
 *   FreeType utility file for memory and list management (body).
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
 */

#include "kernel.h"

/*
 * src/base/ftbitmap.c:36-37
 */
static const FT_Bitmap null_bitmap = {0, 0, 0, NULL, 0, 0, 0, NULL};

/*
 * src/base/ftbitmap.c:42-47
 */
FT_EXPORT_DEF(void)
FT_Bitmap_Init(FT_Bitmap *abitmap) {
    if (abitmap)
        *abitmap = null_bitmap;
}

/*
 * src/base/ftbitmap.c:134-277
 */
static FT_Error ft_bitmap_assure_buffer(FT_Memory memory, FT_Bitmap *bitmap,
                                        FT_UInt xpixels, FT_UInt ypixels) {
    FT_Error error;
    unsigned int pitch;
    unsigned int new_pitch;
    FT_UInt bpp;
    FT_UInt width, height;
    unsigned char *buffer = NULL;

    width = bitmap->width;
    height = bitmap->rows;
    pitch = (unsigned int)FT_ABS(bitmap->pitch);

    switch (bitmap->pixel_mode) {
    case FT_PIXEL_MODE_MONO:
        bpp = 1;
        new_pitch = (width + xpixels + 7) >> 3;
        break;
    case FT_PIXEL_MODE_GRAY2:
        bpp = 2;
        new_pitch = (width + xpixels + 3) >> 2;
        break;
    case FT_PIXEL_MODE_GRAY4:
        bpp = 4;
        new_pitch = (width + xpixels + 1) >> 1;
        break;
    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_LCD:
    case FT_PIXEL_MODE_LCD_V:
        bpp = 8;
        new_pitch = width + xpixels;
        break;
    default:
        return FT_THROW(Invalid_Glyph_Format);
    }

    if (ypixels == 0 && new_pitch <= pitch) {

        FT_UInt bit_width = pitch * 8;
        FT_UInt bit_last = (width + xpixels) * bpp;

        if (bit_last < bit_width) {
            FT_Byte *line = bitmap->buffer + (bit_last >> 3);
            FT_Byte *end = bitmap->buffer + pitch;
            FT_UInt shift = bit_last & 7;
            FT_UInt mask = 0xFF00U >> shift;
            FT_UInt count = height;

            for (; count > 0; count--, line += pitch, end += pitch) {
                FT_Byte *write = line;

                if (shift > 0) {
                    write[0] = (FT_Byte)(write[0] & mask);
                    write++;
                }
                if (write < end)
                    FT_MEM_ZERO(write, end - write);
            }
        }

        return FT_Err_Ok;
    }

    if (FT_QALLOC_MULT(buffer, bitmap->rows + ypixels, new_pitch))
        return error;

    if (bitmap->pitch > 0) {
        FT_UInt len = (width * bpp + 7) >> 3;

        unsigned char *in = bitmap->buffer;
        unsigned char *out = buffer;

        unsigned char *limit = bitmap->buffer + pitch * bitmap->rows;
        unsigned int delta = new_pitch - len;

        FT_MEM_ZERO(out, new_pitch * ypixels);
        out += new_pitch * ypixels;

        while (in < limit) {
            FT_MEM_COPY(out, in, len);
            in += pitch;
            out += len;

            FT_MEM_ZERO(out, delta);
            out += delta;
        }
    } else {
        FT_UInt len = (width * bpp + 7) >> 3;

        unsigned char *in = bitmap->buffer;
        unsigned char *out = buffer;

        unsigned char *limit = bitmap->buffer + pitch * bitmap->rows;
        unsigned int delta = new_pitch - len;

        while (in < limit) {
            FT_MEM_COPY(out, in, len);
            in += pitch;
            out += len;

            FT_MEM_ZERO(out, delta);
            out += delta;
        }

        FT_MEM_ZERO(out, new_pitch * ypixels);
    }

    FT_FREE(bitmap->buffer);
    bitmap->buffer = buffer;

    if (bitmap->pitch < 0)
        bitmap->pitch = -(int)new_pitch;
    else
        bitmap->pitch = (int)new_pitch;

    return FT_Err_Ok;
}

/*
 * src/base/ftbitmap.c:282-439
 */
FT_EXPORT_DEF(FT_Error)
FT_Bitmap_Embolden(FT_Library library, FT_Bitmap *bitmap, FT_Pos xStrength,
                   FT_Pos yStrength) {
    FT_Error error;
    unsigned char *p;
    FT_Int i, x, pitch;
    FT_UInt y;
    FT_Int xstr, ystr;

    if (!library)
        return FT_THROW(Invalid_Library_Handle);

    if (!bitmap || !bitmap->buffer)
        return FT_THROW(Invalid_Argument);

    if (((FT_PIX_ROUND(xStrength) >> 6) > FT_INT_MAX) ||
        ((FT_PIX_ROUND(yStrength) >> 6) > FT_INT_MAX))
        return FT_THROW(Invalid_Argument);

    xstr = (FT_Int)FT_PIX_ROUND(xStrength) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND(yStrength) >> 6;

    if (xstr == 0 && ystr == 0)
        return FT_Err_Ok;
    else if (xstr < 0 || ystr < 0)
        return FT_THROW(Invalid_Argument);

    switch (bitmap->pixel_mode) {
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4: {
        FT_Bitmap tmp;

        FT_Bitmap_Init(&tmp);
        error = FT_Bitmap_Convert(library, bitmap, &tmp, 1);
        if (error)
            return error;

        FT_Bitmap_Done(library, bitmap);
        *bitmap = tmp;
    } break;

    case FT_PIXEL_MODE_MONO:
        if (xstr > 8)
            xstr = 8;
        break;

    case FT_PIXEL_MODE_LCD:
        xstr *= 3;
        break;

    case FT_PIXEL_MODE_LCD_V:
        ystr *= 3;
        break;

    case FT_PIXEL_MODE_BGRA:

        return FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer(library->memory, bitmap, (FT_UInt)xstr,
                                    (FT_UInt)ystr);
    if (error)
        return error;

    pitch = bitmap->pitch;
    if (pitch > 0)
        p = bitmap->buffer + pitch * ystr;
    else {
        pitch = -pitch;
        p = bitmap->buffer + (FT_UInt)pitch * (bitmap->rows - 1);
    }

    for (y = 0; y < bitmap->rows; y++) {

        for (x = pitch - 1; x >= 0; x--) {
            unsigned char tmp;

            tmp = p[x];
            for (i = 1; i <= xstr; i++) {
                if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
                    p[x] |= tmp >> i;

                    if (x > 0)
                        p[x] |= p[x - 1] << (8 - i);

                } else {
                    if (x - i >= 0) {
                        if (p[x] + p[x - i] > bitmap->num_grays - 1) {
                            p[x] = (unsigned char)(bitmap->num_grays - 1);
                            break;
                        } else {
                            p[x] = (unsigned char)(p[x] + p[x - i]);
                            if (p[x] == bitmap->num_grays - 1)
                                break;
                        }
                    } else
                        break;
                }
            }
        }

        for (x = 1; x <= ystr; x++) {
            unsigned char *q;

            q = p - bitmap->pitch * x;
            for (i = 0; i < pitch; i++)
                q[i] |= p[i];
        }

        p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    return FT_Err_Ok;
}

/*
 * src/base/ftbitmap.c:442-485
 */
static FT_Byte ft_gray_for_premultiplied_srgb_bgra(const FT_Byte *bgra) {
    FT_UInt a = bgra[3];
    FT_UInt l;

    if (!a)
        return 0;

    l = (4731UL * bgra[0] * bgra[0] + 46868UL * bgra[1] * bgra[1] +
         13937UL * bgra[2] * bgra[2]) >>
        16;

    return (FT_Byte)(a - l / a);
}

/*
 * src/base/ftbitmap.c:490-760
 */
FT_EXPORT_DEF(FT_Error)
FT_Bitmap_Convert(FT_Library library, const FT_Bitmap *source,
                  FT_Bitmap *target, FT_Int alignment) {
    FT_Error error = FT_Err_Ok;
    FT_Memory memory;

    FT_Byte *s;
    FT_Byte *t;

    if (!library)
        return FT_THROW(Invalid_Library_Handle);

    if (!source || !target)
        return FT_THROW(Invalid_Argument);

    memory = library->memory;

    switch (source->pixel_mode) {
    case FT_PIXEL_MODE_MONO:
    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
    case FT_PIXEL_MODE_LCD:
    case FT_PIXEL_MODE_LCD_V:
    case FT_PIXEL_MODE_BGRA: {
        FT_Int width = (FT_Int)source->width;
        FT_Int neg =
            (target->pitch == 0 && source->pitch < 0) || target->pitch < 0;

        FT_Bitmap_Done(library, target);

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows = source->rows;
        target->width = source->width;

        if (alignment) {
            FT_Int rem = width % alignment;

            if (rem)
                width = alignment > 0 ? width - rem + alignment
                                      : width - rem - alignment;
        }

        if (FT_QALLOC_MULT(target->buffer, target->rows, width))
            return error;

        target->pitch = neg ? -width : width;
    } break;

    default:
        error = FT_THROW(Invalid_Argument);
    }

    s = source->buffer;
    t = target->buffer;

    if (source->pitch < 0)
        s -= source->pitch * (FT_Int)(source->rows - 1);
    if (target->pitch < 0)
        t -= target->pitch * (FT_Int)(target->rows - 1);

    switch (source->pixel_mode) {
    case FT_PIXEL_MODE_MONO: {
        FT_UInt i;

        target->num_grays = 2;

        for (i = source->rows; i > 0; i--) {
            FT_Byte *ss = s;
            FT_Byte *tt = t;
            FT_UInt j;

            for (j = source->width >> 3; j > 0; j--) {
                FT_Int val = ss[0];

                tt[0] = (FT_Byte)((val & 0x80) >> 7);
                tt[1] = (FT_Byte)((val & 0x40) >> 6);
                tt[2] = (FT_Byte)((val & 0x20) >> 5);
                tt[3] = (FT_Byte)((val & 0x10) >> 4);
                tt[4] = (FT_Byte)((val & 0x08) >> 3);
                tt[5] = (FT_Byte)((val & 0x04) >> 2);
                tt[6] = (FT_Byte)((val & 0x02) >> 1);
                tt[7] = (FT_Byte)(val & 0x01);

                tt += 8;
                ss += 1;
            }

            j = source->width & 7;
            if (j > 0) {
                FT_Int val = *ss;

                for (; j > 0; j--) {
                    tt[0] = (FT_Byte)((val & 0x80) >> 7);
                    val <<= 1;
                    tt += 1;
                }
            }

            s += source->pitch;
            t += target->pitch;
        }
    } break;

    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_LCD:
    case FT_PIXEL_MODE_LCD_V: {
        FT_UInt width = source->width;
        FT_UInt i;

        target->num_grays = 256;

        for (i = source->rows; i > 0; i--) {
            FT_ARRAY_COPY(t, s, width);

            s += source->pitch;
            t += target->pitch;
        }
    } break;

    case FT_PIXEL_MODE_GRAY2: {
        FT_UInt i;

        target->num_grays = 4;

        for (i = source->rows; i > 0; i--) {
            FT_Byte *ss = s;
            FT_Byte *tt = t;
            FT_UInt j;

            for (j = source->width >> 2; j > 0; j--) {
                FT_Int val = ss[0];

                tt[0] = (FT_Byte)((val & 0xC0) >> 6);
                tt[1] = (FT_Byte)((val & 0x30) >> 4);
                tt[2] = (FT_Byte)((val & 0x0C) >> 2);
                tt[3] = (FT_Byte)((val & 0x03));

                ss += 1;
                tt += 4;
            }

            j = source->width & 3;
            if (j > 0) {
                FT_Int val = ss[0];

                for (; j > 0; j--) {
                    tt[0] = (FT_Byte)((val & 0xC0) >> 6);
                    val <<= 2;
                    tt += 1;
                }
            }

            s += source->pitch;
            t += target->pitch;
        }
    } break;

    case FT_PIXEL_MODE_GRAY4: {
        FT_UInt i;

        target->num_grays = 16;

        for (i = source->rows; i > 0; i--) {
            FT_Byte *ss = s;
            FT_Byte *tt = t;
            FT_UInt j;

            for (j = source->width >> 1; j > 0; j--) {
                FT_Int val = ss[0];

                tt[0] = (FT_Byte)((val & 0xF0) >> 4);
                tt[1] = (FT_Byte)((val & 0x0F));

                ss += 1;
                tt += 2;
            }

            if (source->width & 1)
                tt[0] = (FT_Byte)((ss[0] & 0xF0) >> 4);

            s += source->pitch;
            t += target->pitch;
        }
    } break;

    case FT_PIXEL_MODE_BGRA: {
        FT_UInt i;

        target->num_grays = 256;

        for (i = source->rows; i > 0; i--) {
            FT_Byte *ss = s;
            FT_Byte *tt = t;
            FT_UInt j;

            for (j = source->width; j > 0; j--) {
                tt[0] = ft_gray_for_premultiplied_srgb_bgra(ss);

                ss += 4;
                tt += 1;
            }

            s += source->pitch;
            t += target->pitch;
        }
    } break;

    default:;
    }

    return error;
}

/*
 * src/base/ftbitmap.c:1108-1127
 */
FT_EXPORT_DEF(FT_Error)
FT_Bitmap_Done(FT_Library library, FT_Bitmap *bitmap) {
    FT_Memory memory;

    if (!library)
        return FT_THROW(Invalid_Library_Handle);

    if (!bitmap)
        return FT_THROW(Invalid_Argument);

    memory = library->memory;

    FT_FREE(bitmap->buffer);
    *bitmap = null_bitmap;

    return FT_Err_Ok;
}

/*
 * src/base/ftutil.c:112-165
 */
FT_BASE_DEF(FT_Pointer)
ft_mem_qrealloc(FT_Memory memory, FT_Long item_size, FT_Long cur_count,
                FT_Long new_count, void *block, FT_Error *p_error) {
    FT_Error error = FT_Err_Ok;

    if (cur_count < 0 || new_count < 0 || item_size < 0) {

        error = FT_THROW(Invalid_Argument);
    } else if (new_count == 0 || item_size == 0) {
        ft_mem_free(memory, block);
        block = NULL;
    } else if (new_count > FT_INT_MAX / item_size) {
        error = FT_THROW(Array_Too_Large);
    } else if (cur_count == 0) {
        FT_ASSERT(!block);

        block = memory->alloc(memory, new_count * item_size);
        if (block == NULL)
            error = FT_THROW(Out_Of_Memory);
    } else {
        FT_Pointer block2;
        FT_Long cur_size = cur_count * item_size;
        FT_Long new_size = new_count * item_size;

        block2 = memory->realloc(memory, cur_size, new_size, block);
        if (!block2)
            error = FT_THROW(Out_Of_Memory);
        else
            block = block2;
    }

    *p_error = error;
    return block;
}

/*
 * src/base/ftutil.c:168-174
 */
FT_BASE_DEF(void)
ft_mem_free(FT_Memory memory, const void *P) {
    if (P)
        memory->free(memory, (void *)P);
}
