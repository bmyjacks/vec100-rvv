#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

const char16_t *decomposeQuickCheck_rvv(const char16_t *, const char16_t *,
                                        const UCPTrie *, char16_t, uint16_t,
                                        uint16_t, bool);

int main() {
    // A full FAST BMP index and 16-bit data array, with the high-value slot
    // for supplementary scalars. Exercise the real UCPTrie indexing macros.
    std::vector<uint16_t> index(1024);
    std::vector<uint16_t> data(65538);
    for (unsigned i = 0; i < index.size(); ++i) index[i] = i * 64;
    UCPTrie trie{};
    trie.index = index.data(); trie.data.ptr16 = data.data();
    trie.indexLength = index.size(); trie.dataLength = data.size();
    trie.highStart = 0x10000; trie.type = UCPTRIE_TYPE_FAST;
    trie.valueWidth = 0;
    std::mt19937 rng(0x15dec0u);
    const char16_t choices[] = {u'A', u'B', 0x007f, 0x0300, 0x0301,
                                0x0340, 0xd800, 0xdc00, 0x11a8, 0xac00};
    for (int trial = 0; trial < 1500; ++trial) {
        for (char16_t c : choices)
            data[c] = uint16_t( (rng() % 5) == 0 ? 0x1000 :
                                (rng() % 4) == 0 ? 0xfc06 :
                                (rng() % 3) == 0 ? 0xfc04 :
                                (rng() % 2) == 0 ? 0xfc00 : 0);
        data[65536] = uint16_t((trial % 3) ? 0x1000 : 0);
        std::vector<char16_t> text;
        unsigned len = trial % 13 == 0 ? 0 : rng() % 530;
        for (unsigned j = 0; j < len; ++j) {
            text.push_back(j < len / 2 && trial % 2 ? char16_t(1 + rng() % 0x40)
                                                         : choices[rng() % 10]);
        }
        text.push_back(0);
        for (char16_t minCP : {char16_t(0), char16_t(0x80), char16_t(0x400)}) {
            for (bool terminated : {false, true}) {
                for (bool error : {false, true}) {
                    const char16_t *begin = text.data();
                    const char16_t *limit = terminated ? nullptr : begin + len;
                    auto ref = decomposeQuickCheck(begin, limit, &trie, minCP,
                                                   0x1000, 0xfc00, error);
                    auto got = decomposeQuickCheck_rvv(begin, limit, &trie, minCP,
                                                       0x1000, 0xfc00, error);
                    if (ref != got) {
                        std::fprintf(stderr, "trial=%d len=%u minCP=%u terminated=%d error=%d offset=%td/%td\n",
                                     trial, len, unsigned(minCP), terminated, error,
                                     ref - begin, got - begin);
                        return 1;
                    }
                }
            }
        }
    }
    std::puts("UTF-16 quick-check differential OK");
}
