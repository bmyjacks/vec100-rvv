/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/OT/glyf/SimpleGlyph.hh
 *
 */

#include "kernel.h"

/*
 * src/OT/glyf/SimpleGlyph.hh:126-147
 */
bool SimpleGlyph::read_flags(const HBUINT8 *&p,
                             hb_array_t<contour_point_t> points_,
                             const HBUINT8 *end) {
    auto *points = points_.arrayZ;
    unsigned count = points_.length;
    for (unsigned int i = 0; i < count;) {
        if (unlikely(p + 1 > end))
            return false;
        uint8_t flag = *p++;
        points[i++].flag = flag;
        if (flag & FLAG_REPEAT) {
            if (unlikely(p + 1 > end))
                return false;
            unsigned int repeat_count = *p++;
            unsigned stop = hb_min(i + repeat_count, count);
            for (; i < stop; i++)
                points[i].flag = flag;
        }
    }
    return true;
}

/*
 * src/OT/glyf/SimpleGlyph.hh:149-178
 */
bool SimpleGlyph::read_points(const HBUINT8 *&p,
                              hb_array_t<contour_point_t> points_,
                              const HBUINT8 *end, float contour_point_t::*m,
                              const simple_glyph_flag_t short_flag,
                              const simple_glyph_flag_t same_flag) {
    int v = 0;

    for (auto &point : points_) {
        unsigned flag = point.flag;
        if (flag & short_flag) {
            if (unlikely(p + 1 > end))
                return false;
            v += (bool(flag & same_flag) * 2 - 1) * *p++;
        } else {
            if (!(flag & same_flag)) {
                if (unlikely(p + HBINT16::static_size > end))
                    return false;
                v += *(const HBINT16 *)p;
                p += HBINT16::static_size;
            }
        }
        point.*m = v;
    }
    return true;
}

/*
 * Wrappers for invoking the extracted kernel.
 */
extern "C" bool read_flags(const HBUINT8 *&p,
                           hb_array_t<contour_point_t> points_,
                           const HBUINT8 *end) {
    return SimpleGlyph::read_flags(p, points_, end);
}

extern "C" bool read_points(const HBUINT8 *&p,
                            hb_array_t<contour_point_t> points_,
                            const HBUINT8 *end, float contour_point_t::*m,
                            const SimpleGlyph::simple_glyph_flag_t short_flag,
                            const SimpleGlyph::simple_glyph_flag_t same_flag) {
    return SimpleGlyph::read_points(p, points_, end, m, short_flag, same_flag);
}
