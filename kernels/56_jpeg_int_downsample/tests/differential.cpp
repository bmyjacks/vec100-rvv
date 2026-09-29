#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

void jpeg_int_downsample_rvv(j_compress_ptr, jpeg_component_info *,
                             _JSAMPARRAY, _JSAMPARRAY);

static unsigned state = 0x1285eeda;
static unsigned random_word() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

struct Plane {
    std::vector<std::vector<JSAMPLE>> input, output;
    std::vector<JSAMPROW> in_rows, out_rows;

    Plane(int rows, int out_rows_count, unsigned capacity, unsigned out_cols,
          unsigned seed, int pattern, int alias)
        : input(rows, std::vector<JSAMPLE>(capacity + 19)),
          output(out_rows_count, std::vector<JSAMPLE>(out_cols + 19, 0xa5)) {
        unsigned s = seed;
        for (int r = 0; r < rows; ++r) {
            for (unsigned c = 0; c < capacity + 19; ++c) {
                s ^= s << 13;
                s ^= s >> 17;
                s ^= s << 5;
                input[r][c] = pattern == 0 ? 0 : pattern == 1 ? 255
                               : pattern == 2 ? ((r + c) & 1 ? 255 : 0)
                               : pattern == 3 ? (r + c) % 5 : s;
            }
            in_rows.push_back(input[r].data() + 3);
        }
        for (int r = 0; r < out_rows_count; ++r)
            out_rows.push_back(output[r].data() + 3);
        if (alias == 1)
            out_rows[0] = in_rows[0];
        else if (alias == 2)
            out_rows[0] = in_rows[rows - 1] + 1;
        else if (alias == 3)
            out_rows[0] = in_rows[0] - 1;
        else if (alias == 4 && out_rows_count > 1)
            out_rows[1] = out_rows[0] + 1;
    }
};

static unsigned cases = 0;
static void check(int maxh, int maxv, int h, int v, bool lossless,
                  unsigned blocks, unsigned width, unsigned seed, int pattern,
                  int alias) {
    jpeg_comp_master master = {static_cast<int>(lossless)};
    jpeg_compress_struct info = {width, maxh, maxv, &master};
    jpeg_component_info comp = {h, v, blocks};
    unsigned cols = blocks * (lossless ? 1 : DCTSIZE);
    unsigned expanded = cols * (maxh / h);
    unsigned capacity = (expanded > width ? expanded : width) + cols + 8;
    Plane scalar(maxv, v, capacity, cols, seed, pattern, alias);
    Plane vector(maxv, v, capacity, cols, seed, pattern, alias);
    jpeg_int_downsample(&info, &comp, scalar.in_rows.data(), scalar.out_rows.data());
    jpeg_int_downsample_rvv(&info, &comp, vector.in_rows.data(), vector.out_rows.data());
    if (scalar.input != vector.input || scalar.output != vector.output) {
        std::fprintf(stderr,
                     "mismatch case=%u seed=%08x max=%d,%d factor=%d,%d lossless=%d "
                     "blocks=%u width=%u pattern=%d alias=%d\n",
                     cases, seed, maxh, maxv, h, v, lossless, blocks, width,
                     pattern, alias);
        std::exit(1);
    }
    ++cases;
}

int main() {
    // One partial horizontal block: final source pixel must be replicated.
    // Two rows with different values also catch averaging the wrong vertical row.
    {
        jpeg_comp_master master = {0};
        jpeg_compress_struct info = {3, 2, 2, &master};
        jpeg_component_info comp = {1, 1, 1};
        JSAMPLE a[16] = {0, 2, 4}, b[16] = {6, 8, 10};
        JSAMPLE out[8] = {};
        JSAMPROW in_rows[] = {a, b}, out_rows[] = {out};
        jpeg_int_downsample_rvv(&info, &comp, in_rows, out_rows);
        const JSAMPLE expected[] = {4, 7, 7, 7, 7, 7, 7, 7};
        for (int i = 0; i < 8; ++i)
            if (out[i] != expected[i] || a[i] != (i < 3 ? 2 * i : 4) ||
                b[i] != (i < 3 ? 6 + 2 * i : 10)) {
                std::fprintf(stderr, "partial-block oracle failed at %d\n", i);
                return 1;
            }
    }
    // Four-way sum of 2 must round up rather than truncate to zero.
    {
        jpeg_comp_master master = {1};
        jpeg_compress_struct info = {2, 2, 2, &master};
        jpeg_component_info comp = {1, 1, 1};
        JSAMPLE a[2] = {2, 0}, b[2] = {0, 0}, out[1] = {};
        JSAMPROW in[] = {a, b}, o[] = {out};
        jpeg_int_downsample_rvv(&info, &comp, in, o);
        if (out[0] != 1) return 1;
    }
    for (int maxh = 1; maxh <= 4; ++maxh)
        for (int maxv = 1; maxv <= 4; ++maxv)
            for (int h = 1; h <= maxh; ++h)
                for (int v = 1; v <= maxv; ++v) {
                    if (maxh % h || maxv % v) continue;
                    for (int lossless = 0; lossless < 2; ++lossless) {
                        for (unsigned cols : {1u, 2u, 3u, 7u, 15u, 16u,
                                              17u, 31u, 32u, 33u, 65u}) {
                            unsigned blocks = lossless ? cols : (cols + 7) / 8;
                            unsigned padded = blocks * (lossless ? 1 : 8) * (maxh / h);
                            unsigned width = 1 + random_word() % padded;
                            int pattern = cases % 5;
                            unsigned seed = random_word();
                            check(maxh, maxv, h, v, lossless, blocks, width,
                                  seed, pattern, 0);
                            if (cols == 17 || cols == 1) {
                                check(maxh, maxv, h, v, lossless, blocks, width,
                                      seed, pattern, 1);
                                check(maxh, maxv, h, v, lossless, blocks, width,
                                      seed, pattern, 2);
                                check(maxh, maxv, h, v, lossless, blocks, width,
                                      seed, pattern, 3);
                                if (v > 1)
                                    check(maxh, maxv, h, v, lossless, blocks, width,
                                          seed, pattern, 4);
                            }
                        }
                    }
                }
    for (unsigned i = 0; i < 400; ++i) {
        int maxh = 1 + random_word() % 4, maxv = 1 + random_word() % 4;
        int h = 1 + random_word() % maxh, v = 1 + random_word() % maxv;
        if (maxh % h || maxv % v) continue;
        bool lossless = random_word() & 1;
        unsigned blocks = 1 + random_word() % 39;
        unsigned padded = blocks * (lossless ? 1 : 8) * (maxh / h);
        check(maxh, maxv, h, v, lossless, blocks,
              1 + random_word() % padded, random_word(), 4, i % 3);
    }
    std::printf("jpeg int_downsample: %u exact cases, seed 0x1285eeda OK\n", cases);
}
