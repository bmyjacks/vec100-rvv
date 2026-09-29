#include "kernel.h"

#include <riscv_vector.h>

namespace {
constexpr guint32 power33(unsigned n) {
    guint32 p = 1;
    for (unsigned i = 0; i < n; ++i) p *= 33u;
    return p;
}

// For sixteen signed input bytes b[i], the block transform is
// h' = h * 33^16 + sum(b[i] * 33^(15-i)) (mod 2^32).
// The powers are fixed, so there is no scalar per-byte hash computation in
// the vector main path. e8m1 and e32m4 have the same maximum lane count.
constexpr guint32 weights[16] = {
    power33(15), power33(14), power33(13), power33(12),
    power33(11), power33(10), power33(9),  power33(8),
    power33(7),  power33(6),  power33(5),  power33(4),
    power33(3),  power33(2),  power33(1),  power33(0)
};

guint32 hash_bounded(const signed char *p, gsize n) {
    guint32 h = 5381;
    constexpr gsize block = 16;
    if (n >= block) {
        // For a smaller RVV VLEN, use the suffix of the weight table:
        // weights[16-vl] = 33^(vl-1). vsetvl is fixed for all iterations.
        const gsize vl = __riscv_vsetvl_e32m4(block);
        const guint32 factor = power33(static_cast<unsigned>(vl));
        const vuint32m4_t coeff = __riscv_vle32_v_u32m4(weights + block - vl, vl);
        const vuint32m1_t zero = __riscv_vmv_v_x_u32m1(0, vl);
        while (n >= block) {
            const vint8m1_t bytes = __riscv_vle8_v_i8m1(p, vl);
            const vint32m4_t signed_words = __riscv_vsext_vf4_i32m4(bytes, vl);
            const vuint32m4_t words = __riscv_vreinterpret_v_i32m4_u32m4(signed_words);
            const vuint32m4_t products = __riscv_vmul_vv_u32m4(words, coeff, vl);
            const vuint32m1_t sum = __riscv_vredsum_vs_u32m4_u32m1(products, zero, vl);
            h = h * factor + __riscv_vmv_x_s_u32m1_u32(sum);
            p += vl;
            n -= vl;
        }
    }
    while (n-- != 0)
        h = (h << 5) + h + *p++;
    return h;
}
} // namespace

guint g_str_hash_rvv(gconstpointer v) {
    const auto *p = static_cast<const signed char *>(v);
    // A NUL-terminated input supplies no readable extent beyond its NUL.
    // Volatile-qualified reads prevent a compiler from widening this probe
    // into a strlen-like scan which could touch bytes after the terminator.
    const volatile signed char *scan = p;
    gsize len = 0;
    while (scan[len] != '\0') ++len;
    return hash_bounded(p, len);
}

guint g_bytes_hash_rvv(gconstpointer bytes) {
    if (bytes == nullptr) return 0;
    const auto *a = static_cast<const GBytes *>(bytes);
    if (a->size == 0) return 5381;
    return hash_bounded(static_cast<const signed char *>(a->data), a->size);
}
