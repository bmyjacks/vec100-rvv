#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>

void update_classbits_rvv(uint32_t, uint32_t, BOOL, uint8_t *);

int main() {
    std::mt19937 rng(0x89c1a55);
    for (unsigned type = 0; type <= PT_PXXDIGIT; ++type) {
        if (type == PT_CLIST) continue; // not an accepted property type here
        for (unsigned data : {0u, 1u, 2u, 5u, 9u, 16u, 29u, 31u, 32u,
                              33u, 55u, 63u, 64u, 99u, 100u, 127u, 158u}) {
            // PT_SCX supports script IDs < ucp_Script_Count; PT_BOOL
            // supports IDs < ucp_Bprop_Count; all tested IDs are in range
            // for the remaining table lookups.
            if ((type == PT_BOOL && data >= ucp_Bprop_Count) ||
                (type == PT_SCX && data >= ucp_Script_Count)) continue;
            for (int neg = 0; neg <= 1; ++neg) {
                for (int rep = 0; rep < 6; ++rep) {
                    uint8_t scalar[48], rvv[48];
                    for (auto &v : scalar) v = rng();
                    std::memcpy(rvv, scalar, sizeof scalar);
                    PRIV(update_classbits)(type, data, neg, scalar + 8);
                    update_classbits_rvv(type, data, neg, rvv + 8);
                    if (std::memcmp(scalar, rvv, sizeof scalar)) {
                        std::fprintf(stderr, "classbits mismatch type=%u data=%u neg=%d rep=%d\n",
                                     type, data, neg, rep);
                        std::abort();
                    }
                }
            }
        }
    }
    std::puts("update_classbits: pass");
}
