#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

UChar *u_strFromUTF8WithSub_rvv(UChar *, int32_t, int32_t *, const char *,
                                int32_t, UChar32, int32_t *, UErrorCode *);

static uint64_t seed = 0xa4b95e01dba18723ULL;
static unsigned random_byte() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return static_cast<unsigned>(seed & 255);
}

static void check(const std::vector<char> &input, int32_t length,
                  int32_t capacity, UChar32 subchar, UErrorCode initial,
                  bool alias, bool report_length, bool report_subs) {
    const size_t units = input.size() + static_cast<size_t>(capacity) + 16;
    std::vector<UChar> a(units, 0x5555), b(units, 0x5555);
    if (alias) {
        std::memcpy(a.data(), input.data(), input.size());
        std::memcpy(b.data(), input.data(), input.size());
    }
    int32_t lengthA = -555, lengthB = -555;
    int32_t subsA = -777, subsB = -777;
    UErrorCode errorA = initial, errorB = initial;
    UChar *outA = capacity ? a.data() : nullptr;
    UChar *outB = capacity ? b.data() : nullptr;
    // For a zero capacity, a non-null destination is also a valid call.
    if (!capacity && alias) { outA = a.data(); outB = b.data(); }
    const char *srcA = alias ? reinterpret_cast<const char *>(a.data()) : input.data();
    const char *srcB = alias ? reinterpret_cast<const char *>(b.data()) : input.data();
    const auto retA = u_strFromUTF8WithSub(
        outA, capacity, report_length ? &lengthA : nullptr, srcA, length,
        subchar, report_subs ? &subsA : nullptr, &errorA);
    const auto retB = u_strFromUTF8WithSub_rvv(
        outB, capacity, report_length ? &lengthB : nullptr, srcB, length,
        subchar, report_subs ? &subsB : nullptr, &errorB);
    if ((retA == nullptr) != (retB == nullptr) || errorA != errorB ||
        lengthA != lengthB || subsA != subsB || a != b) {
        std::fprintf(stderr,
                     "ICU length=%d cap=%d sub=%d initial=%d alias=%d report=%d/%d: "
                     "result=%d/%d error=%d/%d length=%d/%d subs=%d/%d\n",
                     length, capacity, subchar, initial, alias, report_length,
                     report_subs, retA != nullptr, retB != nullptr, errorA,
                     errorB, lengthA, lengthB, subsA, subsB);
        std::exit(1);
    }
}

int main() {
    const int32_t sizes[] = {0, 1, 2, 3, 7, 15, 16, 17, 31, 32, 33,
                             63, 64, 65, 127, 128, 129, 256};
    const UChar32 substitutions[] = {U_SENTINEL, 0, 0xfffd, 0x1f600};
    for (int32_t size : sizes) {
        for (unsigned round = 0; round < 32; ++round) {
            std::vector<char> data(static_cast<size_t>(size) + 5, 'a');
            for (int32_t i = 0; i < size; ++i) {
                if (round % 4 == 0) data[i] = static_cast<char>(random_byte());
                else if (round % 4 == 1) data[i] = static_cast<char>(random_byte() & 0x7f);
            }
            if (size > 3 && round % 4 == 2) {
                const unsigned char codes[][4] = {
                    {0xc2, 0xa2}, {0xe2, 0x82, 0xac},
                    {0xf0, 0x9f, 0x98, 0x80}, {0xe0, 0x80},
                    {0xed, 0xa0, 0x80}, {0xf4, 0x90, 0x80, 0x80}};
                size_t i = size / 2;
                std::memcpy(data.data() + i, codes[round % 6],
                            static_cast<size_t>(size) - i < 4 ? size - i : 4);
            }
            data[size] = 0;
            for (UChar32 sub : substitutions) {
                for (int32_t cap : {0, 1, 2, size / 2, size, size + 1,
                                    size + 4}) {
                    for (int32_t len : {size, -1}) {
                        check(data, len, cap, sub, U_ZERO_ERROR, false, true, true);
                        if (round % 4 == 0) {
                            check(data, len, cap, sub, U_USING_DEFAULT_WARNING,
                                  false, false, false);
                            check(data, len, cap, sub, U_ZERO_ERROR,
                                  false, false, false);
                            check(data, len, cap, sub, U_ZERO_ERROR,
                                  false, true, false);
                            check(data, len, cap, sub, U_ZERO_ERROR,
                                  false, false, true);
                            check(data, len, cap, sub, U_BUFFER_OVERFLOW_ERROR,
                                  false, true, true);
                        }
                        // In-place NUL-terminated conversion can overwrite
                        // the terminator before it is read: it has no bounded
                        // source object to compare once that happens.
                        if (round == 0 && len >= 0 && cap <= size + 1)
                            check(data, len, cap, sub, U_ZERO_ERROR, true, true, true);
                    }
                }
            }
        }
    }
    const std::vector<char> tiny = {'x', 0};
    for (UChar32 invalid : {UChar32(0xd800), UChar32(0x110000)})
        check(tiny, 1, 4, invalid, U_ZERO_ERROR, false, true, true);
    // A readable input ending exactly at a page boundary must not trigger
    // speculative wide loads into the protected page.
    const size_t page = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    void *mem = mmap(nullptr, 2 * page, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED || mprotect(static_cast<char *>(mem) + page,
                                      page, PROT_NONE)) std::abort();
    for (int32_t len : sizes) {
        char *p = static_cast<char *>(mem) + page - len;
        std::memset(p, 'A', len);
        UChar output[300] = {};
        int32_t required = -1, subs = -1;
        UErrorCode error = U_ZERO_ERROR;
        u_strFromUTF8WithSub_rvv(output, 300, &required, p, len, 0xfffd,
                                 &subs, &error);
        if (required != len || subs != 0 || error != U_ZERO_ERROR)
            std::abort();
        if (len) {
            p[len - 1] = static_cast<char>(0xf0);
            error = U_ZERO_ERROR;
            u_strFromUTF8WithSub_rvv(output, 300, &required, p, len, 0xfffd,
                                     &subs, &error);
            if (required != len || subs != 1 || error != U_ZERO_ERROR)
                std::abort();
        }
    }
    munmap(mem, 2 * page);
    std::puts("ICU u_strFromUTF8WithSub: OK");
}
