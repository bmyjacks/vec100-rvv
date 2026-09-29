/****************************************************************************
 *
 *
 *  Project: FreeType 2.14.3
 *  Source files:
 *    src/base/ftoutln.c
 *    src/base/ftbbox.c
 *    src/base/ftcalc.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/base/ftoutln.c
 *
 *   FreeType outline management (body).
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
 * src/base/ftbbox.c
 *
 *   FreeType bbox computation (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used
 * modified and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 * src/base/ftcalc.c
 *
 *   Arithmetic computations (body).
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

#include "kernel.h"

/*
 * src/base/ftoutln.c:42-298
 */
FT_EXPORT_DEF(FT_Error)
FT_Outline_Decompose(FT_Outline *outline,
                     const FT_Outline_Funcs *func_interface, void *user) {
#undef SCALED
#define SCALED(x) ((x) * (1L << shift) - delta)

    FT_Vector v_last;
    FT_Vector v_control;
    FT_Vector v_start;

    FT_Vector *point;
    FT_Vector *limit;
    FT_Byte *tags;

    FT_Error error;

    FT_Int n;
    FT_Int first;
    FT_Int last;

    FT_Int tag;

    FT_Int shift;
    FT_Pos delta;

    if (!outline)
        return FT_THROW(Invalid_Outline);

    if (!func_interface)
        return FT_THROW(Invalid_Argument);

    shift = func_interface->shift;
    delta = func_interface->delta;

    last = -1;
    for (n = 0; n < outline->n_contours; n++) {
        FT_TRACE5(("FT_Outline_Decompose: Contour %d\n", n));

        first = last + 1;
        last = outline->contours[n];
        if (last < first)
            goto Invalid_Outline;

        limit = outline->points + last;

        v_start = outline->points[first];
        v_start.x = SCALED(v_start.x);
        v_start.y = SCALED(v_start.y);

        v_last = outline->points[last];
        v_last.x = SCALED(v_last.x);
        v_last.y = SCALED(v_last.y);

        v_control = v_start;

        point = outline->points + first;
        tags = outline->tags + first;
        tag = FT_CURVE_TAG(tags[0]);

        if (tag == FT_CURVE_TAG_CUBIC)
            goto Invalid_Outline;

        if (tag == FT_CURVE_TAG_CONIC) {

            if (FT_CURVE_TAG(outline->tags[last]) == FT_CURVE_TAG_ON) {

                v_start = v_last;
                limit--;
            } else {

                v_start.x = (v_start.x + v_last.x) / 2;
                v_start.y = (v_start.y + v_last.y) / 2;
            }
            point--;
            tags--;
        }

        FT_TRACE5(("  move to (%.2f, %.2f)\n", (double)v_start.x / 64,
                   (double)v_start.y / 64));
        error = func_interface->move_to(&v_start, user);
        if (error)
            goto Exit;

        while (point < limit) {
            point++;
            tags++;

            tag = FT_CURVE_TAG(tags[0]);
            switch (tag) {
            case FT_CURVE_TAG_ON: {
                FT_Vector vec;

                vec.x = SCALED(point->x);
                vec.y = SCALED(point->y);

                FT_TRACE5(("  line to (%.2f, %.2f)\n", (double)vec.x / 64,
                           (double)vec.y / 64));
                error = func_interface->line_to(&vec, user);
                if (error)
                    goto Exit;
                continue;
            }

            case FT_CURVE_TAG_CONIC:
                v_control.x = SCALED(point->x);
                v_control.y = SCALED(point->y);

            Do_Conic:
                if (point < limit) {
                    FT_Vector vec;
                    FT_Vector v_middle;

                    point++;
                    tags++;
                    tag = FT_CURVE_TAG(tags[0]);

                    vec.x = SCALED(point->x);
                    vec.y = SCALED(point->y);

                    if (tag == FT_CURVE_TAG_ON) {
                        FT_TRACE5(("  conic to (%.2f, %.2f)"
                                   " with control (%.2f, %.2f)\n",
                                   (double)vec.x / 64, (double)vec.y / 64,
                                   (double)v_control.x / 64,
                                   (double)v_control.y / 64));
                        error =
                            func_interface->conic_to(&v_control, &vec, user);
                        if (error)
                            goto Exit;
                        continue;
                    }

                    if (tag != FT_CURVE_TAG_CONIC)
                        goto Invalid_Outline;

                    v_middle.x = (v_control.x + vec.x) / 2;
                    v_middle.y = (v_control.y + vec.y) / 2;

                    FT_TRACE5(("  conic to (%.2f, %.2f)"
                               " with control (%.2f, %.2f)\n",
                               (double)v_middle.x / 64, (double)v_middle.y / 64,
                               (double)v_control.x / 64,
                               (double)v_control.y / 64));
                    error =
                        func_interface->conic_to(&v_control, &v_middle, user);
                    if (error)
                        goto Exit;

                    v_control = vec;
                    goto Do_Conic;
                }

                FT_TRACE5(("  conic to (%.2f, %.2f)"
                           " with control (%.2f, %.2f)\n",
                           (double)v_start.x / 64, (double)v_start.y / 64,
                           (double)v_control.x / 64, (double)v_control.y / 64));
                error = func_interface->conic_to(&v_control, &v_start, user);
                goto Close;

            default: {
                FT_Vector vec1, vec2;

                if (point + 1 > limit ||
                    FT_CURVE_TAG(tags[1]) != FT_CURVE_TAG_CUBIC)
                    goto Invalid_Outline;

                point += 2;
                tags += 2;

                vec1.x = SCALED(point[-2].x);
                vec1.y = SCALED(point[-2].y);

                vec2.x = SCALED(point[-1].x);
                vec2.y = SCALED(point[-1].y);

                if (point <= limit) {
                    FT_Vector vec;

                    vec.x = SCALED(point->x);
                    vec.y = SCALED(point->y);

                    FT_TRACE5(("  cubic to (%.2f, %.2f)"
                               " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                               (double)vec.x / 64, (double)vec.y / 64,
                               (double)vec1.x / 64, (double)vec1.y / 64,
                               (double)vec2.x / 64, (double)vec2.y / 64));
                    error = func_interface->cubic_to(&vec1, &vec2, &vec, user);
                    if (error)
                        goto Exit;
                    continue;
                }

                FT_TRACE5(("  cubic to (%.2f, %.2f)"
                           " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                           (double)v_start.x / 64, (double)v_start.y / 64,
                           (double)vec1.x / 64, (double)vec1.y / 64,
                           (double)vec2.x / 64, (double)vec2.y / 64));
                error = func_interface->cubic_to(&vec1, &vec2, &v_start, user);
                goto Close;
            }
            }
        }

        FT_TRACE5(("  line to (%.2f, %.2f)\n", (double)v_start.x / 64,
                   (double)v_start.y / 64));
        error = func_interface->line_to(&v_start, user);

    Close:
        if (error)
            goto Exit;
    }

    FT_TRACE5(("FT_Outline_Decompose: Done\n"));
    return FT_Err_Ok;

Invalid_Outline:
    error = FT_THROW(Invalid_Outline);

Exit:
    FT_TRACE5(("FT_Outline_Decompose: Error 0x%x\n", error));
    return error;
}

/*
 * src/base/ftbbox.c:36-41
 */
typedef struct TBBox_Rec_ {
    FT_Vector last;
    FT_BBox bbox;

} TBBox_Rec;

/*
 * src/base/ftbbox.c:44-60
 */
#define FT_UPDATE_BBOX(p, bbox)                                                \
    FT_BEGIN_STMNT                                                             \
    if (p->x < bbox.xMin)                                                      \
        bbox.xMin = p->x;                                                      \
    if (p->x > bbox.xMax)                                                      \
        bbox.xMax = p->x;                                                      \
    if (p->y < bbox.yMin)                                                      \
        bbox.yMin = p->y;                                                      \
    if (p->y > bbox.yMax)                                                      \
        bbox.yMax = p->y;                                                      \
    FT_END_STMNT

#define CHECK_X(p, bbox) (p->x < bbox.xMin || p->x > bbox.xMax)

#define CHECK_Y(p, bbox) (p->y < bbox.yMin || p->y > bbox.yMax)

/*
 * src/base/ftbbox.c:85-97
 */
FT_CALLBACK_DEF(int)
BBox_Move_To(const FT_Vector *to, void *user_) {
    TBBox_Rec *user = (TBBox_Rec *)user_;

    FT_UPDATE_BBOX(to, user->bbox);

    user->last = *to;

    return 0;
}

/*
 * src/base/ftbbox.c:122-132
 */
FT_CALLBACK_DEF(int)
BBox_Line_To(const FT_Vector *to, void *user_) {
    TBBox_Rec *user = (TBBox_Rec *)user_;

    user->last = *to;

    return 0;
}

/*
 * src/base/ftbbox.c:162-182
 */
static void BBox_Conic_Check(FT_Pos y1, FT_Pos y2, FT_Pos y3, FT_Pos *min,
                             FT_Pos *max) {

    y1 -= y2;
    y3 -= y2;
    y2 += FT_MulDiv(y1, y3, y1 + y3);

    if (y2 < *min)
        *min = y2;
    if (y2 > *max)
        *max = y2;
}

/*
 * src/base/ftbbox.c:214-242
 */
FT_CALLBACK_DEF(int)
BBox_Conic_To(const FT_Vector *control, const FT_Vector *to, void *user_) {
    TBBox_Rec *user = (TBBox_Rec *)user_;

    FT_UPDATE_BBOX(to, user->bbox);

    if (CHECK_X(control, user->bbox))
        BBox_Conic_Check(user->last.x, control->x, to->x, &user->bbox.xMin,
                         &user->bbox.xMax);

    if (CHECK_Y(control, user->bbox))
        BBox_Conic_Check(user->last.y, control->y, to->y, &user->bbox.yMin,
                         &user->bbox.yMax);

    user->last = *to;

    return 0;
}

/*
 * src/base/ftbbox.c:275-365
 */
static FT_Pos cubic_peak(FT_Pos q1, FT_Pos q2, FT_Pos q3, FT_Pos q4) {
    FT_Pos peak = 0;
    FT_Int shift;

    shift = 27 - FT_MSB((FT_UInt32)(FT_ABS(q1) | FT_ABS(q2) | FT_ABS(q3) |
                                    FT_ABS(q4)));

    if (shift > 0) {

        if (shift > 2)
            shift = 2;

        q1 *= 1 << shift;
        q2 *= 1 << shift;
        q3 *= 1 << shift;
        q4 *= 1 << shift;
    } else {
        q1 >>= -shift;
        q2 >>= -shift;
        q3 >>= -shift;
        q4 >>= -shift;
    }

    while (q2 > 0 || q3 > 0) {

        if (q1 + q2 > q3 + q4) {
            q4 = q4 + q3;
            q3 = q3 + q2;
            q2 = q2 + q1;
            q4 = q4 + q3;
            q3 = q3 + q2;
            q4 = (q4 + q3) >> 3;
            q3 = q3 >> 2;
            q2 = q2 >> 1;
        } else {
            q1 = q1 + q2;
            q2 = q2 + q3;
            q3 = q3 + q4;
            q1 = q1 + q2;
            q2 = q2 + q3;
            q1 = (q1 + q2) >> 3;
            q2 = q2 >> 2;
            q3 = q3 >> 1;
        }

        if (q1 == q2 && q1 >= q3) {
            peak = q1;
            break;
        }
        if (q3 == q4 && q2 <= q4) {
            peak = q4;
            break;
        }
    }

    if (shift > 0)
        peak >>= shift;
    else
        peak <<= -shift;

    return peak;
}

/*
 * src/base/ftbbox.c:368-387
 */
static void BBox_Cubic_Check(FT_Pos p1, FT_Pos p2, FT_Pos p3, FT_Pos p4,
                             FT_Pos *min, FT_Pos *max) {

    if (p2 > *max || p3 > *max)
        *max += cubic_peak(p1 - *max, p2 - *max, p3 - *max, p4 - *max);

    if (p2 < *min || p3 < *min)
        *min -= cubic_peak(*min - p1, *min - p2, *min - p3, *min - p4);
}

/*
 * src/base/ftbbox.c:422-456
 */
FT_CALLBACK_DEF(int)
BBox_Cubic_To(const FT_Vector *control1, const FT_Vector *control2,
              const FT_Vector *to, void *user_) {
    TBBox_Rec *user = (TBBox_Rec *)user_;

    if (CHECK_X(control1, user->bbox) || CHECK_X(control2, user->bbox))
        BBox_Cubic_Check(user->last.x, control1->x, control2->x, to->x,
                         &user->bbox.xMin, &user->bbox.xMax);

    if (CHECK_Y(control1, user->bbox) || CHECK_Y(control2, user->bbox))
        BBox_Cubic_Check(user->last.y, control1->y, control2->y, to->y,
                         &user->bbox.yMin, &user->bbox.yMax);

    user->last = *to;

    return 0;
}

/*
 * src/base/ftbbox.c:459-468
 */
FT_DEFINE_OUTLINE_FUNCS(bbox_interface,

                        (FT_Outline_MoveTo_Func)BBox_Move_To,
                        (FT_Outline_LineTo_Func)BBox_Line_To,
                        (FT_Outline_ConicTo_Func)BBox_Conic_To,
                        (FT_Outline_CubicTo_Func)BBox_Cubic_To, 0, 0)

/*
 * src/base/ftbbox.c:473-539
 */
FT_EXPORT_DEF(FT_Error)
FT_Outline_Get_BBox(FT_Outline *outline, FT_BBox *abbox) {
    FT_BBox cbox = {0x7FFFFFFFL, 0x7FFFFFFFL, -0x7FFFFFFFL, -0x7FFFFFFFL};
    FT_BBox bbox = {0x7FFFFFFFL, 0x7FFFFFFFL, -0x7FFFFFFFL, -0x7FFFFFFFL};
    FT_Vector *vec;
    FT_UShort n;

    if (!abbox)
        return FT_THROW(Invalid_Argument);

    if (!outline)
        return FT_THROW(Invalid_Outline);

    if (outline->n_points == 0 || outline->n_contours == 0) {
        abbox->xMin = abbox->xMax = 0;
        abbox->yMin = abbox->yMax = 0;

        return 0;
    }

    vec = outline->points;

    for (n = 0; n < outline->n_points; n++) {
        FT_UPDATE_BBOX(vec, cbox);

        if (FT_CURVE_TAG(outline->tags[n]) == FT_CURVE_TAG_ON)
            FT_UPDATE_BBOX(vec, bbox);

        vec++;
    }

    if (cbox.xMin < bbox.xMin || cbox.xMax > bbox.xMax ||
        cbox.yMin < bbox.yMin || cbox.yMax > bbox.yMax) {

        FT_Error error;
        TBBox_Rec user;

        user.bbox = bbox;

        error = FT_Outline_Decompose(outline, &bbox_interface, &user);
        if (error)
            return error;

        *abbox = user.bbox;
    } else
        *abbox = bbox;

    return FT_Err_Ok;
}

/*
 * src/base/ftcalc.c:59-68
 */
#define FT_MOVE_SIGN(utype, x, x_unsigned, s)                                  \
    FT_BEGIN_STMNT                                                             \
    if (x < 0) {                                                               \
        x_unsigned = 0U - (utype)x;                                            \
        s = -s;                                                                \
    } else                                                                     \
        x_unsigned = (utype)x;                                                 \
    FT_END_STMNT

/*
 * src/base/ftcalc.c:161-181
 */
FT_EXPORT_DEF(FT_Long)
FT_MulDiv(FT_Long a_, FT_Long b_, FT_Long c_) {
    FT_Int s = 1;
    FT_UInt64 a, b, c, d;
    FT_Long d_;

    FT_MOVE_SIGN(FT_UInt64, a_, a, s);
    FT_MOVE_SIGN(FT_UInt64, b_, b, s);
    FT_MOVE_SIGN(FT_UInt64, c_, c, s);

    d = c > 0 ? (a * b + (c >> 1)) / c : 0x7FFFFFFFUL;

    d_ = (FT_Long)d;

    return s < 0 ? NEG_LONG(d_) : d_;
}
