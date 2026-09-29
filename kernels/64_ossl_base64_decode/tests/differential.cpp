#include "kernel.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr unsigned seed = 0x12905764;
std::mt19937 rng(seed);
unsigned cases = 0;

// Compare the entire allocation, including bytes written before an error,
// untouched suffixes, and bytes of a context that aliases the output.
bool check(const std::string &input, int eof, int alphabet, int shift, bool ctx_in_output = false) {
    constexpr int src = 1024, dst = 4096;
    std::vector<unsigned char> scalar(8192);
    for (auto &b : scalar)
        b = static_cast<unsigned char>(rng());
    std::memcpy(scalar.data() + src, input.data(), input.size());
    std::vector<unsigned char> rvv = scalar;
    EVP_ENCODE_CTX a{}, b{};
    a.flags = b.flags = alphabet == 2 ? EVP_ENCODE_CTX_USE_SRP_ALPHABET : 0;
    const int target = shift == 10000 ? dst : src + shift;
    EVP_ENCODE_CTX *ca = alphabet ? &a : nullptr;
    EVP_ENCODE_CTX *cb = alphabet ? &b : nullptr;
    if (ctx_in_output) {
        ca = reinterpret_cast<EVP_ENCODE_CTX *>(scalar.data() + dst);
        cb = reinterpret_cast<EVP_ENCODE_CTX *>(rvv.data() + dst);
        ca->flags = cb->flags = alphabet == 2 ? 2 : 0;
    }
    const int x = evp_decodeblock_int(ca, scalar.data() + target,
                                      scalar.data() + src, static_cast<int>(input.size()), eof);
    const int y = evp_decodeblock_int_rvv(cb, rvv.data() + target,
                                          rvv.data() + src, static_cast<int>(input.size()), eof);
    ++cases;
    if (x != y || scalar != rvv || (!ctx_in_output && std::memcmp(&a, &b, sizeof a))) {
        std::fprintf(stderr, "mismatch seed=%u case=%u n=%zu eof=%d alphabet=%d shift=%d ctxout=%d ret=%d/%d\n",
                     seed, cases, input.size(), eof, alphabet, shift, ctx_in_output, x, y);
        return false;
    }
    return true;
}

bool known(const char *text, int eof, int alphabet,
           const std::vector<unsigned char> &expected) {
    unsigned char out[64];
    std::memset(out, 0xa5, sizeof out);
    EVP_ENCODE_CTX ctx{};
    ctx.flags = alphabet ? 2 : 0;
    int ret = evp_decodeblock_int_rvv(&ctx, out,
                  reinterpret_cast<const unsigned char *>(text),
                  static_cast<int>(std::strlen(text)), eof);
    if (ret != static_cast<int>(expected.size()) ||
        std::memcmp(out, expected.data(), expected.size()) != 0 ||
        out[expected.size()] != 0xa5) {
        std::fprintf(stderr, "known vector failed: %s eof=%d alphabet=%d\n", text, eof, alphabet);
        return false;
    }
    return true;
}
} // namespace

int main() {
    if (!known("TWFu", -1, 0, {'M','a','n'}) ||
        !known("TWE=", -1, 0, {'M','a',0}) ||
        !known("TQ==", -1, 0, {'M',0,0}) ||
        !known("TWE=", 1, 0, {'M','a'}) ||
        !known("TQ==", 2, 0, {'M'}) ||
        !known("TWF9", -1, 0, {'M','a'}) ||
        !known("TWFuTWFu", 0, 0, {'M','a','n','M','a','n'}) ||
        !known("./01", 0, 2, {0xfb, 0xf0, 0x01}) ||
        !known("+/AB", 0, 0, {0xfb, 0xf0, 0x01}))
        return 1;

    const std::string standard = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const std::string srp = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./";
    for (int alphabet : {0, 1, 2}) {
        const std::string &chars = alphabet == 2 ? srp : standard;
        for (int len : {0, 1, 2, 3, 4, 8, 12, 16, 20, 64, 128, 256, 512, 1024, 2052}) {
            for (int iteration = 0; iteration < 5; ++iteration) {
                std::string s(len, 'A');
                for (char &v : s)
                    v = chars[rng() % chars.size()];
                if (len >= 4 && iteration % 2)
                    s[len - 1] = '=';
                if (len >= 8 && iteration == 3)
                    s[len - 2] = '=';
                if (len >= 12 && iteration == 4)
                    s[4] = '='; // upstream accepts '=' in a non-final quantum
                for (int eof : {-2, -1, 0, 1, 2, 3}) {
                    for (int shift : {10000, -7, -3, 0, 1, 3, 15, 300}) {
                        if (!check(s, eof, alphabet, shift)) return 1;
                    }
                }
                if (len >= 128 && !check(s, -1, alphabet, 10000, true)) return 1;
            }
        }
        // Every byte position of selected multi-quantum streams, including
        // faults before and after VLEN-sized boundaries and in the last block.
        for (int len : {8, 12, 68, 260, 516}) {
            std::string s(len, chars[17]);
            for (int pos = 0; pos < len; ++pos) {
                if (len > 68 && pos != 0 && pos != 1 && pos != 4 &&
                    pos != 63 && pos != 64 && pos != 127 && pos != 128 &&
                    pos != 255 && pos != 256 && pos != len - 5 &&
                    pos != len - 4 && pos != len - 1)
                    continue;
                for (unsigned char bad : {static_cast<unsigned char>('!'),
                                          static_cast<unsigned char>(0x80),
                                          static_cast<unsigned char>(0xff),
                                          static_cast<unsigned char>('\n')}) {
                    s[pos] = static_cast<char>(bad);
                    for (int shift : {10000, 0, 1, -3})
                        if (!check(s, -1, alphabet, shift)) return 1;
                    s[pos] = chars[17];
                }
            }
        }
        for (std::string s : {"", " ", "\tTWFu", "  TWFu\r\n", "TWFu!!!",
                              "TWFu\xff", "TWFu  TQ==", "TQ==\n", "!TWFu",
                              "TWFuT!Fu", "TWFuTWF!", "TWFuTWFu=", "TWFu TWFu"}) {
            for (int shift : {10000, 0, 1, -3})
                for (int eof : {-1, 0, 1, 2})
                    if (!check(s, eof, alphabet, shift)) return 1;
        }
    }
    std::printf("base64 decode: %u full-buffer differential cases (seed %u)\n", cases, seed);
}
