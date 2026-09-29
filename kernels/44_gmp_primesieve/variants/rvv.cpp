#include "kernel.h"
#include <riscv_vector.h>

// step = 2 * prime >= 34. A consecutive scatter can alias a limb when
// step < 64. Separate even/odd multiples: distance within each stream is
// 2 * step >= 68, so indexed loads and stores have unique limb addresses.
// Complete one stream before loading the other, preserving all OR updates.
static void mark_multiples_rvv(mp_ptr array, mp_size_t bits, mp_size_t first,
                               mp_size_t step) {
    const mp_limb_t stride = 2 * static_cast<mp_limb_t>(step);
    for (unsigned parity = 0; parity != 2; ++parity) {
        if (first > bits || (parity && static_cast<mp_limb_t>(step) >
                                            static_cast<mp_limb_t>(bits - first)))
            break;
        mp_limb_t pos = static_cast<mp_limb_t>(first) + parity * step;
        mp_limb_t remaining = (static_cast<mp_limb_t>(bits) - pos) / stride + 1;
        while (remaining) {
            size_t vl = __riscv_vsetvl_e64m1(remaining);
            vuint64m1_t index = __riscv_vid_v_u64m1(vl);
            index = __riscv_vmul_vx_u64m1(index, stride, vl);
            index = __riscv_vadd_vx_u64m1(index, pos, vl);
            vuint64m1_t offsets = __riscv_vsrl_vx_u64m1(index, 3, vl);
            offsets = __riscv_vand_vx_u64m1(offsets, ~mp_limb_t(7), vl);
            vuint64m1_t old = __riscv_vluxei64_v_u64m1(array, offsets, vl);
            vuint64m1_t one = __riscv_vmv_v_x_u64m1(1, vl);
            vuint64m1_t mask = __riscv_vsll_vv_u64m1(one, index, vl);
            __riscv_vsuxei64_v_u64m1(array, offsets,
                                     __riscv_vor_vv_u64m1(old, mask, vl), vl);
            remaining -= vl;
            if (remaining)
                pos += vl * stride;
        }
    }
}

// Local copies of the pinned sieve's scalar control and pattern-fill helpers.
// The scalar oracle in src/kernel.cpp is never included or modified here.
static mp_limb_t id_to_n(mp_limb_t id) { return id * 3 + 1 + (id & 1); }
static mp_limb_t n_fto_bit(mp_limb_t n) { return ((n - 5) | 1) / 3U; }
static mp_limb_t n_cto_bit(mp_limb_t n) { return (n | 1) / 3U - 1; }

#define SET_OFF1(m1, m2, M1, M2, off, BITS)                                    \
    if (off) {                                                                 \
        if (off < GMP_LIMB_BITS) {                                             \
            m1 = (M1 >> off) | (M2 << (GMP_LIMB_BITS - off));                  \
            if (off <= BITS - GMP_LIMB_BITS) {                                 \
                m2 = M1 << (BITS - GMP_LIMB_BITS - off) | M2 >> off;           \
            } else {                                                           \
                m1 |= M1 << (BITS - off);                                      \
                m2 = M1 >> (off + GMP_LIMB_BITS - BITS);                       \
            }                                                                  \
        } else {                                                               \
            m1 = M1 << (BITS - off) | M2 >> (off - GMP_LIMB_BITS);             \
            m2 = M2 << (BITS - off) | M1 >> (off + GMP_LIMB_BITS - BITS);      \
        }                                                                      \
    } else {                                                                   \
        m1 = M1;                                                               \
        m2 = M2;                                                               \
    }

#define SET_OFF2(m1, m2, m3, M1, M2, M3, off, BITS)                            \
    if (off) {                                                                 \
        if (off <= GMP_LIMB_BITS) {                                            \
            m1 = M2 << (GMP_LIMB_BITS - off);                                  \
            m2 = M3 << (GMP_LIMB_BITS - off);                                  \
            if (off != GMP_LIMB_BITS) {                                        \
                m1 |= (M1 >> off);                                             \
                m2 |= (M2 >> off);                                             \
            }                                                                  \
            if (off <= BITS - 2 * GMP_LIMB_BITS) {                             \
                m3 = M1 << (BITS - 2 * GMP_LIMB_BITS - off) | M3 >> off;       \
            } else {                                                           \
                m2 |= M1 << (BITS - GMP_LIMB_BITS - off);                      \
                m3 = M1 >> (off + 2 * GMP_LIMB_BITS - BITS);                   \
            }                                                                  \
        } else if (off < 2 * GMP_LIMB_BITS) {                                  \
            m1 =                                                               \
                M2 >> (off - GMP_LIMB_BITS) | M3 << (2 * GMP_LIMB_BITS - off); \
            if (off <= BITS - GMP_LIMB_BITS) {                                 \
                m2 = M3 >> (off - GMP_LIMB_BITS) |                             \
                     M1 << (BITS - GMP_LIMB_BITS - off);                       \
                m3 = M2 << (BITS - GMP_LIMB_BITS - off);                       \
                if (off != BITS - GMP_LIMB_BITS) {                             \
                    m3 |= M1 >> (off + 2 * GMP_LIMB_BITS - BITS);              \
                }                                                              \
            } else {                                                           \
                m1 |= M1 << (BITS - off);                                      \
                m2 = M2 << (BITS - off) | M1 >> (GMP_LIMB_BITS - BITS + off);  \
                m3 = M2 >> (GMP_LIMB_BITS - BITS + off);                       \
            }                                                                  \
        } else {                                                               \
            m1 = M1 << (BITS - off) | M3 >> (off - 2 * GMP_LIMB_BITS);         \
            m2 = M2 << (BITS - off) | M1 >> (off + GMP_LIMB_BITS - BITS);      \
            m3 = M3 << (BITS - off) | M2 >> (off + GMP_LIMB_BITS - BITS);      \
        }                                                                      \
    } else {                                                                   \
        m1 = M1;                                                               \
        m2 = M2;                                                               \
        m3 = M3;                                                               \
    }

#define ROTATE1(m1, m2, BITS)                                                  \
    do {                                                                       \
        mp_limb_t __tmp;                                                       \
        __tmp = m1 >> (2 * GMP_LIMB_BITS - BITS);                              \
        m1 = (m1 << (BITS - GMP_LIMB_BITS)) | m2;                              \
        m2 = __tmp;                                                            \
    } while (0)

#define ROTATE2(m1, m2, m3, BITS)                                              \
    do {                                                                       \
        mp_limb_t __tmp;                                                       \
        __tmp = m2 >> (3 * GMP_LIMB_BITS - BITS);                              \
        m2 = m2 << (BITS - GMP_LIMB_BITS * 2) |                                \
             m1 >> (3 * GMP_LIMB_BITS - BITS);                                 \
        m1 = m1 << (BITS - GMP_LIMB_BITS * 2) | m3;                            \
        m3 = __tmp;                                                            \
    } while (0)

static mp_limb_t fill_bitpattern(mp_ptr bit_array, mp_size_t limbs,
                                 mp_limb_t offset) {
    mp_limb_t m11, m12, m21, m22, m23;

    {
        mp_limb_t off1 = offset % (11 * 5 * 2);
        SET_OFF1(m11, m12, SIEVE_MASK1, SIEVE_MASKT, off1, 11 * 5 * 2);
        offset %= 13 * 7 * 2;
        SET_OFF2(m21, m22, m23, SIEVE_2MSK1, SIEVE_2MSK2, SIEVE_2MSKT, offset,
                 13 * 7 * 2);
    }

    do {
        bit_array[0] = m11 | m21;
        if (--limbs == 0)
            break;
        ROTATE1(m11, m12, 11 * 5 * 2);
        bit_array[1] = m11 | m22;
        bit_array += 2;
        ROTATE1(m11, m12, 11 * 5 * 2);
        ROTATE2(m21, m22, m23, 13 * 7 * 2);
    } while (--limbs != 0);
    return n_cto_bit(13 + 1);
}

static void block_resieve_rvv(mp_ptr bit_array, mp_size_t limbs,
                               mp_limb_t offset, mp_srcptr sieve) {
    mp_size_t bits, off = offset;
    mp_limb_t mask, i;

    ASSERT(limbs > 0);
    bits = limbs * GMP_LIMB_BITS - 1;
    i = fill_bitpattern(bit_array, limbs, offset);
    ASSERT(i < GMP_LIMB_BITS);

    mask = CNST_LIMB(1) << i;
    do {
        ++i;
        if ((*sieve & mask) == 0) {
            mp_size_t step, lindex;
            step = id_to_n(i);
            lindex = i * (step + 1) - 1 + (-(i & 1) & (i + 1));
            if (lindex > bits + off)
                break;
            step <<= 1;
            if (lindex < off)
                lindex += step * ((off - lindex - 1) / step + 1);
            lindex -= off;
            mark_multiples_rvv(bit_array, bits, lindex, step);

            lindex = i * (i * 3 + 6) + (i & 1);
            if (lindex < off)
                lindex += step * ((off - lindex - 1) / step + 1);
            lindex -= off;
            mark_multiples_rvv(bit_array, bits, lindex, step);
        }
        mask = mask << 1 | mask >> (GMP_LIMB_BITS - 1);
        sieve += mask & 1;
    } while (1);
}

#define BLOCK_SIZE 2048

// Local popcount closes the RVV object without linking the scalar oracle.
static mp_bitcnt_t popcount_rvv(mp_srcptr up, mp_size_t n) {
    mp_bitcnt_t result = 0;
    for (mp_size_t i = 0; i < n; ++i)
        result += __builtin_popcountl(up[i]);
    return result;
}

mp_limb_t gmp_primesieve_rvv(mp_ptr bit_array, mp_limb_t n) {
    mp_size_t size;
    mp_limb_t bits;
    static mp_limb_t presieved[] = {PRIMESIEVE_INIT_TABLE};

    ASSERT(n > 4);
    bits = n_fto_bit(n);
    size = bits / GMP_LIMB_BITS + 1;
    for (mp_size_t j = 0, lim = MIN(size, PRIMESIEVE_NUMBEROF_TABLE); j < lim;
         ++j)
        bit_array[j] = presieved[j];
    if (size > PRIMESIEVE_NUMBEROF_TABLE) {
        mp_size_t off;
        off = size > 2 * BLOCK_SIZE ? BLOCK_SIZE + (size % BLOCK_SIZE) : size;
        block_resieve_rvv(bit_array + PRIMESIEVE_NUMBEROF_TABLE,
                          off - PRIMESIEVE_NUMBEROF_TABLE,
                          GMP_LIMB_BITS * PRIMESIEVE_NUMBEROF_TABLE, bit_array);
        for (; off < size; off += BLOCK_SIZE)
            block_resieve_rvv(bit_array + off, BLOCK_SIZE, off * GMP_LIMB_BITS,
                               bit_array);
    }
    if ((bits + 1) % GMP_LIMB_BITS != 0)
        bit_array[size - 1] |= MP_LIMB_T_MAX << ((bits + 1) % GMP_LIMB_BITS);
    return size * GMP_LIMB_BITS - popcount_rvv(bit_array, size);
}
