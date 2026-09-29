#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

bool validate_utf8_rvv(const char *, size_t);

static uint64_t seed = 0xf58e5a18237b6c49ULL;
static unsigned random_byte() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return static_cast<unsigned>(seed & 255);
}

static void check(const char *p, size_t len) {
    const bool expected = validate_utf8_isolated(p, len);
    const bool actual = validate_utf8_rvv(p, len);
    if (expected != actual) {
        std::fprintf(stderr, "simdjson len=%zu expected=%d actual=%d\n",
                     len, expected, actual);
        std::exit(1);
    }
}

int main() {
    const unsigned char samples[][5] = {
        {0}, {0xc2, 0x80}, {0xdf, 0xbf}, {0xe0, 0xa0, 0x80},
        {0xed, 0x9f, 0xbf}, {0xef, 0xbf, 0xbf}, {0xf0, 0x90, 0x80, 0x80},
        {0xf4, 0x8f, 0xbf, 0xbf}, {0xc0, 0x80}, {0xe0, 0x9f, 0xbf},
        {0xed, 0xa0, 0x80}, {0xf0, 0x8f, 0xbf, 0xbf},
        {0xf4, 0x90, 0x80, 0x80}, {0xff}, {0x80}, {0xe2, 0x82},
        {0xf0, 0x90, 0x80}, {0xe2, 0x00, 0xa0}, {0xf1, 0x80, 0x41, 0x80}};
    const size_t widths[] = {0, 1, 2, 7, 15, 16, 17, 31, 32, 33,
                             63, 64, 65, 127, 128, 129, 255, 1024};
    for (size_t len : widths) {
        for (unsigned shift = 0; shift < 8; ++shift) {
            std::vector<char> buf(len + shift + 8, 'a');
            char *p = buf.data() + shift;
            check(p, len);
            for (size_t at : {size_t(0), size_t(1), size_t(15), size_t(16),
                              size_t(31), size_t(32), len / 2,
                              len ? len - 1 : size_t(0)}) {
                if (at >= len) continue;
                for (const auto &seq : samples) {
                    for (size_t n = 1; n <= 4 && at + n <= len; ++n) {
                        std::memcpy(p + at, seq, n);
                        check(p, len);
                        std::memset(p + at, 'a', n);
                    }
                }
            }
            for (unsigned round = 0; round < 64; ++round) {
                for (size_t i = 0; i < len; ++i)
                    p[i] = static_cast<char>(random_byte());
                check(p, len);
                for (size_t i = 0; i < len; ++i)
                    p[i] = static_cast<char>(random_byte() & 0x7f);
                check(p, len);
            }
        }
    }
    for (const char *cp : {"\xc2\xa2", "\xe2\x82\xac",
                           "\xf0\x9f\x98\x80"}) {
        const size_t n = std::strlen(cp);
        std::vector<char> p;
        for (unsigned i = 0; i < 300; ++i) {
            p.insert(p.end(), cp, cp + n);
            check(p.data(), p.size());
        }
        for (size_t cut = 1; cut < n; ++cut)
            check(p.data(), p.size() - cut);
    }
    const size_t page = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    void *mem = mmap(nullptr, page * 2, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED || mprotect(static_cast<char *>(mem) + page,
                                      page, PROT_NONE)) std::abort();
    for (size_t len : widths) {
        char *p = static_cast<char *>(mem) + page - len;
        std::memset(p, 'x', len);
        check(p, len);
        if (len) {
            p[len - 1] = static_cast<char>(0xf0);
            check(p, len);
            p[len - 1] = static_cast<char>(0x80);
            check(p, len);
        }
    }
    munmap(mem, page * 2);
    std::puts("simdjson validate_utf8: OK");
}
