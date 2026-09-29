#include "kernel.h"
#include <cstring>
#include <cstdio>
#include <initializer_list>

void gcm_init_4bit_isolated_rvv(u128 *, const u64 *);
void gcm_gmult_4bit_isolated_rvv(u64 *, const u128 *);
void gcm_ghash_4bit_isolated_rvv(u64 *, const u128 *, const u8 *, size_t);
static unsigned state = 0x4a2d61u;
static unsigned rnd() { state ^= state << 13; state ^= state >> 17; state ^= state << 5; return state; }
int main() {
    for (int t = 0; t < 200; ++t) {
        u64 h[2] = {(u64(rnd()) << 32) | rnd(), (u64(rnd()) << 32) | rnd()};
        u128 ref[16], got[16];
        gcm_init_4bit_isolated(ref, h);
        gcm_init_4bit_isolated_rvv(got, h);
        if (memcmp(ref, got, sizeof(ref))) {
            fprintf(stderr, "GHASH init mismatch trial=%d\n", t);
            return 1;
        }
        u64 a[2] = {(u64(rnd()) << 32) | rnd(), (u64(rnd()) << 32) | rnd()}, b[2];
        memcpy(b, a, sizeof(a));
        gcm_gmult_4bit_isolated(a, ref);
        gcm_gmult_4bit_isolated_rvv(b, got);
        if (memcmp(a, b, sizeof(a))) {
            fprintf(stderr, "GHASH multiply mismatch trial=%d scalar=%016llx:%016llx rvv=%016llx:%016llx\n",
                    t, (unsigned long long)a[0], (unsigned long long)a[1],
                    (unsigned long long)b[0], (unsigned long long)b[1]);
            return 1;
        }
        u8 data[16 * 17]; for (auto &x : data) x = rnd();
        for (size_t blocks : {size_t(1), size_t(2), size_t(5), size_t(17)}) {
            memcpy(a, h, sizeof(a)); memcpy(b, a, sizeof(a));
            gcm_ghash_4bit_isolated(a, ref, data, blocks * 16);
            gcm_ghash_4bit_isolated_rvv(b, got, data, blocks * 16);
            if (memcmp(a, b, sizeof(a))) {
                fprintf(stderr, "GHASH mismatch trial=%d blocks=%zu scalar=%016llx:%016llx rvv=%016llx:%016llx\n",
                        t, blocks, (unsigned long long)a[0], (unsigned long long)a[1],
                        (unsigned long long)b[0], (unsigned long long)b[1]);
                return 1;
            }
        }
    }
    puts("GHASH OK");
}
