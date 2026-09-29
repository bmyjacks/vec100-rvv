#include "kernel.h"

#include <assert.h>
#include <riscv_vector.h>
#include <stdint.h>
#include <stdlib.h>

extern "C" struct lh_table *lh_table_new_rvv(int size, lh_entry_free_fn *free_fn,
                                               lh_hash_fn *hash_fn,
                                               lh_equal_fn *equal_fn) {
    assert(size > 0);
    struct lh_table *t = (struct lh_table *)calloc(1, sizeof(struct lh_table));
    if (!t)
        return NULL;

    t->count = 0;
    t->size = size;
    t->table = (struct lh_entry *)calloc(size, sizeof(struct lh_entry));
    if (!t->table) {
        free(t);
        return NULL;
    }
    t->free_fn = free_fn;
    t->hash_fn = hash_fn;
    t->equal_fn = equal_fn;

    // Only k is nonzero after calloc. Strided stores leave every other field zero.
    static_assert(sizeof(uintptr_t) == sizeof(t->table[0].k), "RV64 pointer width");
    size_t remaining = (size_t)size;
    char *keys = (char *)&t->table[0].k;
    const uint64_t empty = (uint64_t)(uintptr_t)LH_EMPTY;
    while (remaining != 0) {
        size_t vl = __riscv_vsetvl_e64m1(remaining);
        vuint64m1_t values = __riscv_vmv_v_x_u64m1(empty, vl);
        __riscv_vsse64_v_u64m1((uint64_t *)keys, (ptrdiff_t)sizeof(struct lh_entry),
                                values, vl);
        keys += vl * sizeof(struct lh_entry);
        remaining -= vl;
    }
    return t;
}
