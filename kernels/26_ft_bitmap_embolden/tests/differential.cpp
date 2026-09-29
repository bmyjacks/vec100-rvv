#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" FT_Error FT_Bitmap_Embolden_rvv(FT_Library, FT_Bitmap *, FT_Pos, FT_Pos);
static void *allocate(FT_Memory, long n) { return std::malloc(n); }
static void release(FT_Memory, void *p) { std::free(p); }
static void *resize(FT_Memory, long, long n, void *p) { return std::realloc(p, n); }
static unsigned long long seed = 0x279a173afe8806b5ULL;
static unsigned rnd() {
    seed ^= seed << 13; seed ^= seed >> 7; seed ^= seed << 17;
    return (unsigned)seed;
}

static void check(FT_Library library, int mode, unsigned width, unsigned rows,
                  int pitch, FT_Pos xs, FT_Pos ys) {
    const size_t bytes = (size_t)(pitch < 0 ? -pitch : pitch) * rows;
    FT_Bitmap a{}, b{};
    a.width = b.width = width;
    a.rows = b.rows = rows;
    a.pitch = b.pitch = pitch;
    a.pixel_mode = b.pixel_mode = mode;
    a.num_grays = b.num_grays = mode == FT_PIXEL_MODE_GRAY2 ? 4 :
          mode == FT_PIXEL_MODE_GRAY4 ? 16 : mode == FT_PIXEL_MODE_MONO ? 2 : 256;
    a.buffer = (FT_Byte *)std::malloc(bytes);
    b.buffer = (FT_Byte *)std::malloc(bytes);
    if (!a.buffer || !b.buffer) std::abort();
    for (size_t i = 0; i < bytes; ++i) {
        unsigned v = rnd();
        if (mode == FT_PIXEL_MODE_GRAY2 || mode == FT_PIXEL_MODE_GRAY4 ||
            mode == FT_PIXEL_MODE_MONO) v &= 255;
        a.buffer[i] = b.buffer[i] = (FT_Byte)v;
    }
    FT_Error ea = FT_Bitmap_Embolden(library, &a, xs, ys);
    FT_Error eb = FT_Bitmap_Embolden_rvv(library, &b, xs, ys);
    const size_t outbytes = (size_t)(a.pitch < 0 ? -a.pitch : a.pitch) * a.rows;
    if (ea != eb || a.width != b.width || a.rows != b.rows ||
        a.pitch != b.pitch || a.pixel_mode != b.pixel_mode ||
        a.num_grays != b.num_grays ||
        (!ea && std::memcmp(a.buffer, b.buffer, outbytes))) {
        std::fprintf(stderr, "embolden mode=%d width=%u rows=%u pitch=%d xs=%ld ys=%ld err=%d/%d\n",
                     mode, width, rows, pitch, xs, ys, ea, eb);
        std::exit(1);
    }
    FT_Bitmap_Done(library, &a);
    FT_Bitmap_Done(library, &b);
}
int main() {
    FT_MemoryRec_ memory = {nullptr, allocate, release, resize};
    FT_LibraryRec_ library = {&memory};
    for (unsigned width : {1u, 2u, 7u, 15u, 16u, 17u, 31u, 32u, 65u, 129u}) {
        for (unsigned rows : {1u, 2u, 5u}) {
            for (int spare : {0, 1, 3, 17}) {
                for (int sign : {-1, 1}) {
                    int pitch = sign * (int)(width + spare);
                    for (int trial = 0; trial < 5; ++trial)
                        check(&library, FT_PIXEL_MODE_GRAY, width, rows,
                              pitch, 64, 0);
                    check(&library, FT_PIXEL_MODE_GRAY, width, rows, pitch, 32, -31);
                    check(&library, FT_PIXEL_MODE_GRAY, width, rows, pitch, 95, 31);
                    check(&library, FT_PIXEL_MODE_GRAY, width, rows, pitch, 128, 0);
                    check(&library, FT_PIXEL_MODE_GRAY, width, rows, pitch, 64, 64);
                }
            }
        }
    }
    for (int mode : {FT_PIXEL_MODE_MONO, FT_PIXEL_MODE_GRAY2,
                     FT_PIXEL_MODE_GRAY4, FT_PIXEL_MODE_LCD,
                     FT_PIXEL_MODE_LCD_V, FT_PIXEL_MODE_BGRA}) {
        const int pitch = mode == FT_PIXEL_MODE_MONO ? 5 :
                          mode == FT_PIXEL_MODE_GRAY2 ? 9 :
                          mode == FT_PIXEL_MODE_GRAY4 ? 17 : 34;
        check(&library, mode, 32, 3, pitch, 64, 64);
    }
    std::puts("ft_bitmap_embolden: OK");
}
