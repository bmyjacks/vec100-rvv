#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

void SlopeLimiting_rvv(cmsToneCurve *);

static unsigned long long state = 0x1dafe821bb72957dULL;
static unsigned random_word() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return static_cast<unsigned>(state);
}

static void check(unsigned n, unsigned mode) {
    std::vector<cmsUInt16Number> a(n + 2, 0xa55a), b(n + 2, 0xa55a);
    for (unsigned i = 0; i < n; ++i) {
        a[i + 1] = mode == 0 ? 0 : mode == 1 ? 65535 :
                   mode == 2 ? static_cast<cmsUInt16Number>(i * 65535u / (n - 1)) :
                   mode == 3 ? static_cast<cmsUInt16Number>((n - 1 - i) * 65535u / (n - 1)) :
                   mode == 4 ? (i & 1 ? 0 : 65535) :
                   mode == 6 ? static_cast<cmsUInt16Number>(32768 + (i & 1)) :
                   static_cast<cmsUInt16Number>(random_word());
    }
    b = a;
    cmsToneCurve ref{};
    ref.nEntries = n;
    ref.Table16 = a.data() + 1;
    cmsToneCurve actual{};
    actual.nEntries = n;
    actual.Table16 = b.data() + 1;
    SlopeLimiting_isolated(&ref);
    SlopeLimiting_rvv(&actual);
    for (unsigned i = 0; i < n + 2; ++i) {
        if (a[i] != b[i]) {
            std::fprintf(stderr, "slopelimiting n=%u mode=%u i=%u: %u != %u\n",
                         n, mode, i, b[i], a[i]);
            std::exit(1);
        }
    }
}

int main() {
    for (unsigned n = 1; n <= 513; ++n)
        for (unsigned mode = 0; mode < 7; ++mode) check(n, mode);
    for (unsigned n : {1023u, 1024u, 1025u, 2048u, 4096u, 8192u, 16384u})
        for (unsigned mode = 0; mode < 7; ++mode) check(n, mode);
    for (unsigned t = 0; t < 1000; ++t)
        check(25 + random_word() % 2048, 5);
    std::puts("slopelimiting: OK");
}
