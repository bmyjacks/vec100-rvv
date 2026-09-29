#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>

int encode_mcu_AC_refine_prepare_rvv(const JCOEF *, const int *, int, int,
                                        UJCOEF *, size_t *);

// src/jutils.c:59-67: the actual encoder zigzag order (without decoder padding).
static const int zigzag[64] = {
  0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5,
  12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28,
  35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
  58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};

int main()
{
  const unsigned seed = 0x138ac026U;
  std::mt19937 rng(seed);
  unsigned cases = 0;
  for (int Al : {0, 1, 2, 7, 9, 12})
    for (int Ss : {1, 2, 17, 31, 32, 33, 48, 62, 63})
      for (int Sl : {1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33, 47, 62, 63}) {
        if (Ss + Sl > 64) continue;
        for (int trial = 0; trial < 20; ++trial) {
          JCOEF block[64];
          int order[64];
          for (int j = 0; j < 64; ++j) {
            order[j] = trial % 5 == 0 ? zigzag[Ss + (j % Sl)] :
                       trial % 5 == 1 ? j : int(rng() % 64);
            int v = trial % 5 == 0 ? (j % 4 == 0 ? -32768 : j % 4 == 1 ? 32767 :
                                        j % 4 == 2 ? -1 : 0) :
                    trial % 5 == 1 ? (j % 3 - 1) * (1 << Al) :
                    trial % 5 == 2 ? (j % 5 - 2) * (1 << Al) :
                    int(rng() % 65536) - 32768;
            block[j] = static_cast<JCOEF>(v);
          }
          UJCOEF a[68], b[68];
          for (int j = 0; j < 68; ++j) a[j] = b[j] = static_cast<UJCOEF>(rng());
          size_t ab[4] = {~size_t(0), ~size_t(0), 0x129, 0x541};
          size_t bb[4];
          std::memcpy(bb, ab, sizeof(ab));
          int ea = encode_mcu_AC_refine_prepare(block, order, Sl, Al, a, ab);
          int eb = encode_mcu_AC_refine_prepare_rvv(block, order, Sl, Al, b, bb);
          if (ea != eb || std::memcmp(a, b, sizeof(a)) ||
              std::memcmp(ab, bb, sizeof(ab))) {
            std::fprintf(stderr, "seed=%x case=%u Ss=%d Sl=%d Al=%d trial=%d EOB=%d/%d\n",
                         seed, cases, Ss, Sl, Al, trial, ea, eb);
            return 1;
          }
          ++cases;
        }
      }

  // A last newly-nonzero coefficient at index zero and no newly-nonzero
  // coefficient both return zero; distinguish them using the exact bitmaps.
  JCOEF edge[64] = {};
  UJCOEF a[68] = {}, b[68] = {};
  size_t ab[2], bb[2];
  for (int at : {0, 30, 31, 61}) {
    std::memset(edge, 0, sizeof(edge));
    edge[zigzag[at + 1]] = -1;
    edge[zigzag[at + 2]] = 2;
    int ea = encode_mcu_AC_refine_prepare(edge, zigzag + 1, at + 2, 0, a, ab);
    int eb = encode_mcu_AC_refine_prepare_rvv(edge, zigzag + 1, at + 2, 0, b, bb);
    if (ea != eb || std::memcmp(a, b, sizeof(a)) ||
        std::memcmp(ab, bb, sizeof(ab))) return 2;
  }

  // An overlapping output can alter a later coefficient: require scalar
  // traversal, including for repeated zigzag indices.
  alignas(8) UJCOEF x[192], y[192];
  int repeated[63];
  for (int j = 0; j < 192; ++j) x[j] = y[j] = static_cast<UJCOEF>(rng());
  for (int j = 0; j < 63; ++j) repeated[j] = (j % 4 == 0) ? 0 : j;
  int ea = encode_mcu_AC_refine_prepare(reinterpret_cast<JCOEF *>(x),
                                                repeated, 63, 2, x, ab);
  int eb = encode_mcu_AC_refine_prepare_rvv(reinterpret_cast<JCOEF *>(y),
                                         repeated, 63, 2, y, bb);
  if (ea != eb || std::memcmp(x, y, sizeof(x)) ||
      std::memcmp(ab, bb, sizeof(ab))) return 3;
  // The bitmap destination can intersect the magnitude workspace in a
  // standalone call.  The upstream callback writes both bitmaps LAST.
  for (int j = 0; j < 192; ++j) x[j] = y[j] = static_cast<UJCOEF>(rng());
  ea = encode_mcu_AC_refine_prepare(edge, zigzag + 1, 63, 1, x,
                                            reinterpret_cast<size_t *>(x + 4));
  eb = encode_mcu_AC_refine_prepare_rvv(edge, zigzag + 1, 63, 1, y,
                                     reinterpret_cast<size_t *>(y + 4));
  if (ea != eb || std::memcmp(x, y, sizeof(x))) return 4;
  // Empty standalone band: writes both zero bitmaps, touches no coefficients.
  std::memset(edge, 0, sizeof(edge));
  ea = encode_mcu_AC_refine_prepare(edge, zigzag + 1, 0, 0, a, ab);
  eb = encode_mcu_AC_refine_prepare_rvv(edge, zigzag + 1, 0, 0, b, bb);
  if (ea != eb || std::memcmp(a, b, sizeof(a)) ||
      std::memcmp(ab, bb, sizeof(ab))) return 5;
  std::printf("jpeg-ac-refine: %u exact cases + boundaries + overlap (seed %x)\n",
              cases, seed);
}
