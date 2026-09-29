#include "kernel.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

static constexpr unsigned seed = 0x49a119e;
static unsigned cases;

static void fail(const char *name, unsigned mode, size_t n) {
    std::fprintf(stderr, "seed=%x case=%u %s mode=%u n=%zu\n",
                 seed, cases, name, mode, n);
    std::abort();
}

int main() {
    std::mt19937 rng(seed);
    for (size_t n : {size_t(0), size_t(1), size_t(3), size_t(4), size_t(8),
                     size_t(15), size_t(16), size_t(19), size_t(20), size_t(31),
                     size_t(35), size_t(36), size_t(51), size_t(128), size_t(513)}) {
        for (unsigned mode = 0; mode < 5; ++mode) {
            std::vector<BYTE> bytes(std::max<size_t>(n, 8));
            for (size_t i = 0; i < n; ++i)
                bytes[i] = mode == 0 ? 0 : mode == 1 ? 255 : mode == 2 ?
                           static_cast<BYTE>(i & 1) : mode == 3 ?
                           static_cast<BYTE>(i % 5) : static_cast<BYTE>(rng());

            std::array<unsigned, 256> addA, addB;
            for (size_t i = 0; i < 256; ++i)
                addA[i] = addB[i] = i & 1 ? rng() : UINT32_MAX - 4;
            HIST_add_isolated(addA.data(), bytes.data(), n);
            HIST_add_rvv(addB.data(), bytes.data(), n);
            if (addA != addB)
                fail("add", mode, n);
            ++cases;

            std::array<unsigned, 320> simpleA, simpleB;
            simpleA.fill(0xcafebabe);
            simpleB = simpleA;
            unsigned limitA = 255, limitB = limitA;
            const unsigned ra = HIST_count_simple_isolated(simpleA.data(), &limitA,
                                                           bytes.data(), n);
            const unsigned rb = HIST_count_simple_rvv(simpleB.data(), &limitB,
                                                   bytes.data(), n);
            if (ra != rb || limitA != limitB || simpleA != simpleB)
                fail("simple", mode, n);
            ++cases;

            // The simple API accepts a table whose stated maximum is above
            // the byte alphabet; those trailing bins must remain zero.
            simpleA.fill(0xcafebabe);
            simpleB = simpleA;
            limitA = limitB = 300;
            const unsigned wideA = HIST_count_simple_isolated(simpleA.data(), &limitA,
                                                               bytes.data(), n);
            const unsigned wideB = HIST_count_simple_rvv(simpleB.data(), &limitB,
                                                      bytes.data(), n);
            if (wideA != wideB || limitA != limitB || simpleA != simpleB)
                fail("simple max 300", mode, n);
            ++cases;

            for (unsigned limit : {0u, 4u, 127u, 255u}) {
                for (HIST_checkInput_e check : {trustInput, checkMaxSymbolValue}) {
                    std::array<U32, HIST_WKSP_SIZE_U32> wa, wb;
                    std::array<unsigned, 260> ca, cb;
                    ca.fill(0xbadc0ffe);
                    cb = ca;
                    wa.fill(0xdeadbeef);
                    wb = wa;
                    unsigned ma = limit, mb = limit;
                    const size_t ra = HIST_count_parallel_wksp_bench(
                        ca.data(), &ma, bytes.data(), n, check, wa.data());
                    const size_t rb = HIST_count_parallel_wksp_bench_rvv(
                        cb.data(), &mb, bytes.data(), n, check, wb.data());
                    if (ra != rb || ma != mb || ca != cb || wa != wb)
                        fail("parallel", mode * 8 + limit, n);
                    ++cases;
                }
            }
        }
    }

    // The source is also the output: zeroing / successive increments must
    // affect subsequent reads exactly as in the original loop.
    for (size_t n : {size_t(0), size_t(4), size_t(20), size_t(40)}) {
        std::array<unsigned, 260> a{}, b{};
        a[0] = b[0] = 255;
        for (size_t i = 1; i < a.size(); ++i)
            a[i] = b[i] = rng() & 0xff;
        const unsigned ra = HIST_count_simple_isolated(a.data(), a.data(), a.data(), n);
        const unsigned rb = HIST_count_simple_rvv(b.data(), b.data(), b.data(), n);
        if (ra != rb || a != b)
            fail("simple alias count/max/source", 0, n);
        ++cases;
    }
    {
        std::array<unsigned, 256> a{}, b{};
        a[0] = b[0] = 0;
        HIST_add_isolated(a.data(), a.data(), 60);
        HIST_add_rvv(b.data(), b.data(), 60);
        if (a != b)
            fail("add alias", 0, 60);
        ++cases;
    }
    for (size_t n : {size_t(0), size_t(4), size_t(20), size_t(53)}) {
        std::array<U32, HIST_WKSP_SIZE_U32> wa{}, wb{};
        std::array<unsigned, 260> ca{}, cb{};
        ca[0] = cb[0] = 255;
        for (size_t j = 1; j < ca.size(); ++j)
            ca[j] = cb[j] = rng();
        for (size_t j = 0; j < wa.size(); ++j)
            wa[j] = wb[j] = rng();
        const size_t ra = HIST_count_parallel_wksp_bench(
            ca.data(), ca.data(), wa.data(), n, trustInput, wa.data());
        const size_t rb = HIST_count_parallel_wksp_bench_rvv(
            cb.data(), cb.data(), wb.data(), n, trustInput, wb.data());
        if (ra != rb || ca != cb || wa != wb)
            fail("parallel alias source/workspace/max/count", 0, n);
        ++cases;
    }
    for (size_t n : {size_t(0), size_t(4), size_t(20), size_t(53)}) {
        std::array<U32, HIST_WKSP_SIZE_U32> wa{}, wb{};
        wa[0] = wb[0] = 255;
        std::array<BYTE, 60> bytes{};
        for (size_t j = 0; j < bytes.size(); ++j)
            bytes[j] = static_cast<BYTE>(rng());
        const size_t ra = HIST_count_parallel_wksp_bench(
            wa.data() + 10, wa.data(), bytes.data(), n, checkMaxSymbolValue, wa.data());
        const size_t rb = HIST_count_parallel_wksp_bench_rvv(
            wb.data() + 10, wb.data(), bytes.data(), n, checkMaxSymbolValue, wb.data());
        if (ra != rb || wa != wb)
            fail("parallel alias count/workspace/max", 0, n);
        ++cases;
    }
    std::printf("hist: %u exact cases (seed %x)\n", cases, seed);
}
