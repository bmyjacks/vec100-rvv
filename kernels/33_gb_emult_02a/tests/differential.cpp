#include "kernel.h"
#include <cstdio>
#include <vector>

int main() {
    uint32_t seed = 331;
    auto rnd = [&]() { seed = seed * 1664525u + 1013904223u; return seed; };
    const int vlen = 97;
    std::vector<uint32_t> ap32{0, 73, 159, 227};
    std::vector<uint64_t> ap64(ap32.begin(), ap32.end());
    std::vector<uint32_t> ah32{2, 0, 1};
    std::vector<uint64_t> ah64(ah32.begin(), ah32.end());
    std::vector<int32_t> ai32(227);
    for (int k = 0; k < 3; ++k)
        for (int p = ap32[k]; p < int(ap32[k + 1]); ++p)
            ai32[p] = (p - ap32[k]) % vlen;
    std::vector<int64_t> ai64(ai32.begin(), ai32.end());
    std::vector<double> ax(227), bx(vlen * 3);
    std::vector<int8_t> bb(bx.size());
    for (auto &x : ax) x = int(rnd() % 89) * .125;
    for (auto &x : bx) x = int(rnd() % 71) * .25;
    for (auto &x : bb) x = rnd() % 4 ? 0 : -1;
    for (bool wide : {false, true}) for (bool hyper : {false, true})
        for (bool aiso : {false, true}) for (bool biso : {false, true}) {
        std::vector<uint32_t> cp32(4);
        for (int k = 0; k < 3; ++k) {
            int j = hyper ? ah32[k] : k;
            cp32[k + 1] = cp32[k];
            for (int p = ap32[k]; p < int(ap32[k + 1]); ++p)
                cp32[k + 1] += bb[j * vlen + ai32[p]] != 0;
        }
        std::vector<uint64_t> cp64(cp32.begin(), cp32.end());
        std::vector<double> ref(246), out(246);
        std::vector<int32_t> ci32ref(246), ci32out(246);
        std::vector<int64_t> ci64ref(246), ci64out(246);
        for (auto &x : ref) x = rnd() % 17;
        out = ref;
        for (auto &x : ci32ref) x = rnd();
        ci32out = ci32ref;
        for (auto &x : ci64ref) x = rnd();
        ci64out = ci64ref;
        // Task 0 covers first half of vector 0. Phase-one Cp and Cp_kfirst
        // count precisely the live bitmap entries in each output slice.
        int64_t first[] = {0, 0}, last[] = {0, 2};
        int64_t start[] = {0, 38, 227};
        int64_t cfirst[] = {0, 0};
        for (int p = 0; p < 38; ++p)
            cfirst[1] += bb[(hyper ? ah32[0] : 0) * vlen + ai32[p]] != 0;
        auto call = [&](auto fn, double *cx, int32_t *ci32, int64_t *ci64) {
            fn(wide ? nullptr : ai32.data(), wide ? ai64.data() : nullptr,
               wide ? nullptr : ap32.data(), wide ? ap64.data() : nullptr,
               hyper && !wide ? ah32.data() : nullptr,
               hyper && wide ? ah64.data() : nullptr,
               wide ? nullptr : cp32.data(), wide ? cp64.data() : nullptr,
               ax.data(), bx.data(), bb.data(), cx, ci32, ci64,
               first, last, start, cfirst, vlen, 2, 1, aiso, biso);
        };
        if (wide) {
            call(GB_emult_02a_template, ref.data(), nullptr, ci64ref.data());
            call(GB_emult_02a_template_rvv, out.data(), nullptr, ci64out.data());
        } else {
            call(GB_emult_02a_template, ref.data(), ci32ref.data(), nullptr);
            call(GB_emult_02a_template_rvv, out.data(), ci32out.data(), nullptr);
        }
        if (ref != out || ci32ref != ci32out || ci64ref != ci64out) {
            std::fprintf(stderr, "emult mismatch wide=%d hyper=%d aiso=%d biso=%d values=%d indices32=%d indices64=%d\n",
                         wide, hyper, aiso, biso, ref != out, ci32ref != ci32out, ci64ref != ci64out);
            return 1;
        }
    }
}
