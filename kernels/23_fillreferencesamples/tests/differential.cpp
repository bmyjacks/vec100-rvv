#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

void fillReferenceSamples_rvv(const pixel *, intptr_t, const Predict::IntraNeighbors &, pixel *);
static unsigned long long seed = 0x24659e735b88fa01ULL;
static unsigned rnd() {
    seed ^= seed << 13; seed ^= seed >> 7; seed ^= seed << 17;
    return (unsigned)seed;
}

int main() {
    const int stride = 512;
    std::vector<pixel> image(stride * 512);
    for (pixel &p : image) p = (pixel)rnd();
    const pixel *origin = image.data() + 150 * stride + 150;
    for (int log2 : {2, 3, 4, 5, 6}) {
        const int size = 1 << log2;
        for (int width : {1, 2, 4, 8, 16}) {
          for (int height : {1, 2, 4, 8, 16}) {
            if (width > size || height > size ||
                2 * size / width + 2 * size / height + 1 >
                    4 * MAX_NUM_SPU_W + 1) continue;
            for (int trial = 0; trial < 90; ++trial) {
                Predict::IntraNeighbors nb{};
                nb.log2TrSize = log2;
                nb.unitWidth = width;
                nb.unitHeight = height;
                nb.leftUnits = 2 * size / height;
                nb.aboveUnits = 2 * size / width;
                nb.totalUnits = nb.leftUnits + nb.aboveUnits + 1;
                for (int i = 0; i < nb.totalUnits; ++i) {
                    nb.bNeighborFlags[i] = trial == 0 ||
                         (trial != 1 && rnd() % 3 == 0);
                    nb.numIntraNeighbor += nb.bNeighborFlags[i];
                }
                // A partially available boundary has at least one valid unit.
                if (trial > 1 && !nb.numIntraNeighbor) {
                    nb.bNeighborFlags[nb.totalUnits - 1] = true;
                    nb.numIntraNeighbor = 1;
                }
                pixel a[258], b[258];
                std::memset(a, 0xa5, sizeof a);
                std::memset(b, 0xa5, sizeof b);
                const int step = trial % 2 ? stride : -stride;
                Predict::fillReferenceSamples(origin, step, nb, a);
                fillReferenceSamples_rvv(origin, step, nb, b);
                if (std::memcmp(a, b, sizeof a)) {
                    std::fprintf(stderr, "fillReferenceSamples log2=%d width=%d height=%d trial=%d\n", log2, width, height, trial);
                    return 1;
                }
            }
          }
        }
    }
    std::puts("fillReferenceSamples: OK");
}
