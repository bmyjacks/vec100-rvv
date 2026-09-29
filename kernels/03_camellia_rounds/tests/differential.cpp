#include "../include/kernel.h"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
extern "C" void Camellia_EncryptBlock_Rounds_pinned(int, const u8 *,
                                                    const u32 *, u8 *);
#endif

static u32 state = 0x7b5c013dU;
static u32 next() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
static bool check(int rounds, int mode, int trial) {
    struct Inputs {
        u8 before[32];
        KEY_TABLE_TYPE key;
        u8 after[32];
    } a{}, b{};
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
    Inputs c{};
#endif
    for (u8 &v : a.before)
        v = (u8)next();
    for (u8 &v : a.after)
        v = (u8)next();
    for (u32 &v : a.key)
        v = next();
    if (trial == 0) {
        std::memset(a.key, 0, sizeof a.key);
        std::memset(a.before, 0, sizeof a.before);
    }
    if (trial == 1) {
        std::memset(a.key, 255, sizeof a.key);
        std::memset(a.before, 255, sizeof a.before);
    }
    b = a;
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
    c = a;
#endif
    const u8 *pa = a.before + 7, *pb = b.before + 7;
    u8 *qa = a.after + 5, *qb = b.after + 5;
    if (mode == 1) {
        qa = a.before + 7;
        qb = b.before + 7;
    }
    if (mode == 2) {
        qa = a.before + 9;
        qb = b.before + 9;
    }
    if (mode == 3) {
        qa = reinterpret_cast<u8 *>(a.key) + 4;
        qb = reinterpret_cast<u8 *>(b.key) + 4;
    }
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
    const u8 *pc = c.before + 7;
    u8 *qc = c.after + 5;
    if (mode == 1)
        qc = c.before + 7;
    if (mode == 2)
        qc = c.before + 9;
    if (mode == 3)
        qc = reinterpret_cast<u8 *>(c.key) + 4;
#endif
    Camellia_EncryptBlock_Rounds(rounds, pa, a.key, qa);
    Camellia_EncryptBlock_Rounds_rvv(rounds, pb, b.key, qb);
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
    Camellia_EncryptBlock_Rounds_pinned(rounds, pc, c.key, qc);
#endif
    if (std::memcmp(&a, &b, sizeof a)
#ifdef HAVE_PINNED_CAMELLIA_ORACLE
        || std::memcmp(&a, &c, sizeof a)
#endif
    ) {
        std::fprintf(stderr,
                     "mismatch rounds=%d mode=%d trial=%d seed=0x7b5c013d\n",
                     rounds, mode, trial);
        return false;
    }
    return true;
}
int main() {
    for (int r : {1, 2, 3, 4})
        for (int mode = 0; mode < 4; ++mode)
            for (int t = 0; t < 250; ++t)
                if (!check(r, mode, t))
                    return 1;
    std::puts("Camellia: 4000 differential cases passed (seed=0x7b5c013d)");
}
