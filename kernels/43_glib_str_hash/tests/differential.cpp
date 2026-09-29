#include "kernel.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

namespace {
std::uint64_t state = 0x1375381332026ULL;
std::uint64_t random64() {
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return state * 2685821657736338717ULL;
}

std::size_t cases = 0;
std::uint32_t oracle(const unsigned char *data, std::size_t len) {
    std::uint32_t h = 5381;
    for (std::size_t i = 0; i < len; ++i) {
        const int byte = data[i] < 128 ? data[i] : static_cast<int>(data[i]) - 256;
        h = h * 33u + static_cast<std::uint32_t>(byte);
    }
    return h;
}

void check_bytes(const unsigned char *data, std::size_t n) {
    const GBytes bytes{data, n, 1, nullptr, nullptr};
    const auto expect = oracle(data, n);
    const auto scalar = g_bytes_hash(&bytes);
    const auto rvv = g_bytes_hash_rvv(&bytes);
    if (scalar != expect || rvv != expect) {
        std::fprintf(stderr, "bytes case %zu n=%zu: %08x %08x expected %08x\n",
                     cases, n, scalar, rvv, expect);
        std::exit(1);
    }
    ++cases;
}

void check_str(const unsigned char *data, std::size_t n) {
    std::size_t len = 0;
    while (len < n && data[len] != 0) ++len;
    const auto expect = oracle(data, len);
    const auto scalar = g_str_hash(data);
    const auto rvv = g_str_hash_rvv(data);
    if (scalar != expect || rvv != expect) {
        std::fprintf(stderr, "string case %zu n=%zu terminated=%zu: %08x %08x expected %08x\n",
                     cases, n, len, scalar, rvv, expect);
        std::exit(1);
    }
    ++cases;
}

void guard_page_cases() {
    const long page = sysconf(_SC_PAGESIZE);
    if (page < 2048) std::exit(2);
    void *region = mmap(nullptr, static_cast<std::size_t>(page) * 2,
                        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) std::exit(2);
    auto *end = static_cast<unsigned char *>(region) + page;
    if (mprotect(end, static_cast<std::size_t>(page), PROT_NONE) != 0) std::exit(2);
    for (std::size_t n : {0u, 1u, 15u, 16u, 17u, 31u, 32u, 33u, 127u, 255u, 1023u}) {
        auto *str = end - n - 1;
        for (std::size_t i = 0; i < n; ++i) str[i] = static_cast<unsigned char>(0x80u | (i % 127u));
        str[n] = 0;
        check_str(str, n + 1);  // NUL is the last readable byte.
        check_bytes(end - n, n); // The data range ends at the protected page.
    }
    // A NUL at the very first byte must not cause any vector read past it.
    end[-1] = 0;
    check_str(end - 1, 1);
    if (munmap(region, static_cast<std::size_t>(page) * 2) != 0) std::exit(2);
}
} // namespace

int main() {
    if (g_bytes_hash(nullptr) != 0 || g_bytes_hash_rvv(nullptr) != 0 ||
        g_str_hash("") != 5381 || g_str_hash_rvv("") != 5381)
        return 1;
    const GBytes empty{nullptr, 0, 1, nullptr, nullptr};
    if (g_bytes_hash(&empty) != 5381 || g_bytes_hash_rvv(&empty) != 5381)
        return 1;

    const std::size_t lengths[] = {0, 1, 2, 3, 7, 15, 16, 17, 31, 32, 33,
                                   47, 48, 49, 63, 64, 65, 127, 128, 129,
                                   255, 256, 257, 511, 512, 513, 1024, 4096};
    for (std::size_t n : lengths) {
        for (unsigned offset = 0; offset < 8; ++offset) {
            std::vector<unsigned char> buf(n + offset + 3, 0xa5);
            auto *p = buf.data() + offset;
            for (unsigned pattern = 0; pattern < 8; ++pattern) {
                for (std::size_t i = 0; i < n; ++i) {
                    switch (pattern) {
                    case 0: p[i] = 0; break;
                    case 1: p[i] = 0xff; break;
                    case 2: p[i] = 0x80; break;
                    case 3: p[i] = 0x7f; break;
                    case 4: p[i] = static_cast<unsigned char>(i % 256); break;
                    case 5: p[i] = static_cast<unsigned char>(1 + i % 255); break;
                    default: p[i] = static_cast<unsigned char>(random64()); break;
                    }
                }
                p[n] = 0;
                const auto before = buf;
                check_bytes(p, n); // Include embedded NULs and signed bytes.
                check_str(p, n + 1); // Stops at the first NUL, not at n.
                if (buf != before) std::exit(1);
            }
        }
    }
    for (std::size_t trial = 0; trial < 2500; ++trial) {
        const std::size_t n = trial < 256 ? trial : random64() % 8192;
        std::vector<unsigned char> buf(n + 2);
        for (std::size_t i = 0; i < n; ++i)
            buf[i] = static_cast<unsigned char>(1 + random64() % 255);
        buf[n] = 0;
        buf[n + 1] = 0xfe;
        const auto before = buf;
        check_bytes(buf.data(), n);
        check_str(buf.data(), n + 1);
        if (n != 0) {
            buf[n / 2] = 0;
            check_str(buf.data(), n + 1);
        }
        if (buf[n] != before[n] || buf[n + 1] != before[n + 1]) std::exit(1);
    }
    guard_page_cases();
    std::printf("%zu seeded exact scalar/RVV/oracle cases passed\n", cases);
}
