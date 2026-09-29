#include "kernel.h"

#include <cassert>
#include <cstring>
#include <cstdio>
#include <random>
#include <vector>

int CountVarintsAssumingLargeArray_rvv(const char *, const char *);

int main() {
    std::mt19937 rng(0x90b17);
    for (int length = 8; length <= 1031; ++length) {
        for (int offset = 0; offset < 8; ++offset) {
            std::vector<char> data(length + offset + 16, 0x55);
            for (int i = 0; i < length; ++i)
                data[offset + i] = static_cast<char>(rng());
            const char *p = data.data() + offset;
            for (int pattern = 0; pattern < 3; ++pattern) {
                if (pattern == 1)
                    std::memset(data.data() + offset, 0, length);
                if (pattern == 2)
                    std::memset(data.data() + offset, 0xff, length);
                const int expected = CountVarintsAssumingLargeArray(p, p + length);
                const int actual = CountVarintsAssumingLargeArray_rvv(p, p + length);
                if (expected != actual) {
                    std::fprintf(stderr, "length=%d offset=%d pattern=%d expected=%d got=%d\n",
                                 length, offset, pattern, expected, actual);
                    return 1;
                }
            }
        }
    }
    std::puts("count-varints differential OK");
}
