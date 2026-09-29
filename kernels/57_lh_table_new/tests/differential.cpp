#include "kernel.h"

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

extern "C" lh_table *lh_table_new_rvv(int, lh_entry_free_fn *, lh_hash_fn *, lh_equal_fn *);

static void release(lh_entry *) {}
static unsigned long hash_key(const void *) { return 1; }
static int equal_key(const void *, const void *) { return 1; }

static uint64_t state = 0x59a1b47d238c6ef0ULL;
static uint32_t random32() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return (uint32_t)state;
}

static void check(int size, bool callbacks) {
    lh_entry_free_fn *f = callbacks ? release : nullptr;
    lh_hash_fn *h = callbacks ? hash_key : nullptr;
    lh_equal_fn *e = callbacks ? equal_key : nullptr;
    lh_table *scalar = lh_table_new_isolated(size, f, h, e);
    lh_table *vector = lh_table_new_rvv(size, f, h, e);
    if (!scalar || !vector || scalar == vector || scalar->table == vector->table ||
        scalar->size != vector->size || scalar->count != vector->count ||
        scalar->head != vector->head || scalar->tail != vector->tail ||
        scalar->free_fn != vector->free_fn || scalar->hash_fn != vector->hash_fn ||
        scalar->equal_fn != vector->equal_fn ||
        std::memcmp(scalar->table, vector->table, size * sizeof(lh_entry)) != 0) {
        std::fprintf(stderr, "lh_table_new size=%d callbacks=%d seed=0x59a1b47d238c6ef0\n",
                     size, callbacks);
        std::exit(1);
    }
    for (int i = 0; i < size; ++i) {
        if (vector->table[i].k != LH_EMPTY || vector->table[i].v != nullptr ||
            vector->table[i].next != nullptr || vector->table[i].prev != nullptr ||
            vector->table[i].k_is_constant != 0) std::abort();
    }
    std::free(scalar->table);
    std::free(scalar);
    std::free(vector->table);
    std::free(vector);
}

int main() {
    for (int size : {1, 2, 3, 4, 7, 8, 9, 15, 16, 17, 31, 32, 33, 64, 65,
                     127, 128, 129, 257, 1024, 4097}) {
        check(size, false);
        check(size, true);
    }
    for (int i = 0; i < 300; ++i) check(1 + (random32() % 4000), i & 1);
    std::puts("lh_table_new: OK");
}
