#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

#define DECLARE(bits)                                                           \
    gsize find_unordered_guint##bits##_isolated(const guint8 *, gsize, gsize);  \
    gsize find_unordered_guint##bits##_rvv(const guint8 *, gsize, gsize);
DECLARE(8)
DECLARE(16)
DECLARE(32)
DECLARE(64)
#undef DECLARE

static unsigned long long state = 0x7e9a3c215bd6480fULL;
static unsigned long long random_word() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

template <typename T>
static void check(const char *name,
                  gsize (*scalar)(const guint8 *, gsize, gsize),
                  gsize (*rvv)(const guint8 *, gsize, gsize)) {
    const gsize lengths[] = {1, 2, 3, 4, 7, 8, 15, 16, 17, 31,
                             32, 33, 63, 64, 65, 127, 128, 129, 511};
    for (gsize len : lengths) {
        for (unsigned shift = 0; shift < 9; ++shift) {
            std::vector<guint8> storage(len * sizeof(T) + 16);
            guint8 *data = storage.data() + shift;
            for (unsigned mode = 0; mode < 5; ++mode) {
                for (gsize i = 0; i < len; ++i) {
                    T value = mode == 0 ? static_cast<T>(i) :
                              mode == 1 ? static_cast<T>(len - i) :
                              mode == 2 ? static_cast<T>(7) :
                              mode == 3 ? static_cast<T>(random_word()) :
                                          static_cast<T>(i / 3);
                    memcpy(data + i * sizeof(T), &value, sizeof(T));
                }
                for (gsize start = 0; start < len; ++start) {
                    const gsize expected = scalar(data, start, len);
                    const gsize actual = rvv(data, start, len);
                    if (actual != expected) {
                        std::fprintf(stderr, "%s len=%zu start=%zu shift=%u mode=%u: %zu != %zu\n",
                                     name, len, start, shift, mode, actual, expected);
                        std::exit(1);
                    }
                }
            }
        }
    }
    // Force a late decrease after several vector iterations, including one
    // immediately across a strip boundary, and at different start indices.
    for (unsigned shift = 0; shift < 9; ++shift) {
        constexpr gsize len = 1025;
        std::vector<guint8> storage(len * sizeof(T) + 16);
        guint8 *data = storage.data() + shift;
        for (gsize drop : {gsize(1), gsize(16), gsize(17), gsize(32),
                           gsize(127), gsize(128), gsize(256), gsize(1024)}) {
            for (gsize i = 0; i < len; ++i) {
                const T value = static_cast<T>(i <= drop ? i + 1 : i - drop);
                memcpy(data + i * sizeof(T), &value, sizeof(T));
            }
            const T zero = 0;
            memcpy(data + drop * sizeof(T), &zero, sizeof(T));
            for (gsize start : {gsize(0), drop - 1, drop, gsize(1024)}) {
                const gsize expected = scalar(data, start, len);
                const gsize actual = rvv(data, start, len);
                const bool known_drop = start < drop &&
                    (sizeof(T) > 1 || drop < 250);
                if (actual != expected ||
                    (known_drop && actual != drop - 1) ||
                    (start == len - 1 && actual != len - 1)) {
                    std::fprintf(stderr, "%s late drop=%zu start=%zu shift=%u\n",
                                 name, drop, start, shift);
                    std::exit(1);
                }
            }
        }
    }
    // A final element at the page boundary catches vector tail overreads.
    const gsize page = static_cast<gsize>(sysconf(_SC_PAGESIZE));
    void *region = mmap(nullptr, 2 * page, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED || mprotect(static_cast<char *>(region) + page,
                                        page, PROT_NONE)) std::abort();
    for (gsize len : {gsize(1), gsize(2), gsize(17), gsize(127)}) {
        guint8 *data = static_cast<guint8 *>(region) + page - len * sizeof(T);
        for (gsize i = 0; i < len; ++i) {
            const T value = static_cast<T>(i);
            memcpy(data + i * sizeof(T), &value, sizeof(T));
        }
        if (rvv(data, 0, len) != scalar(data, 0, len) ||
            rvv(data, 0, len) != len - 1) std::abort();
    }
    munmap(region, 2 * page);
}

int main() {
    check<guint8>("u8", find_unordered_guint8_isolated, find_unordered_guint8_rvv);
    check<guint16>("u16", find_unordered_guint16_isolated, find_unordered_guint16_rvv);
    check<guint32>("u32", find_unordered_guint32_isolated, find_unordered_guint32_rvv);
    check<guint64>("u64", find_unordered_guint64_isolated, find_unordered_guint64_rvv);
    std::puts("find_unordered: OK");
}
