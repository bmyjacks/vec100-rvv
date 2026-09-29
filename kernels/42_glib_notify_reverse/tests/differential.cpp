#include "kernel.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

// No GParamSpec fields are used by the kernel; instances provide distinct
// genuine object addresses (and allow inspecting identity after the callback).
struct _GParamSpec {
    unsigned id;
};

struct Dispatch {
    unsigned calls = 0;
    std::vector<GParamSpec *> received;
};

static void dispatch_properties_changed(Dispatch &dispatch, unsigned n_pspecs,
                                        GParamSpec **pspecs) {
    ++dispatch.calls;
    dispatch.received.assign(pspecs, pspecs + n_pspecs);
}

static void check(unsigned len, unsigned mode, std::mt19937 &rng,
                  unsigned case_no) {
    std::array<_GParamSpec, 257> objects{};
    for (unsigned i = 0; i < objects.size(); ++i)
        objects[i].id = i;

    constexpr unsigned guard = 3;
    std::vector<GParamSpec *> scalar(len + 2 * guard);
    for (auto &p : scalar) {
        const unsigned id = rng() % objects.size();
        p = &objects[id];
    }
    for (unsigned i = guard; i < guard + len; ++i) {
        if (mode == 1)
            scalar[i] = &objects[(i - guard) % objects.size()];
        if (mode == 2)
            scalar[i] = &objects[7];
        if (mode == 3 && (i - guard) % 3 == 0)
            scalar[i] = nullptr;
        if (mode == 4)
            scalar[i] = nullptr;
    }
    const std::vector<GParamSpec *> original = scalar;
    std::vector<GParamSpec *> rvv = scalar;
    auto *a = scalar.data() + guard;
    auto *b = rvv.data() + guard;
    glib_notify_reverse(a, static_cast<guint16>(len));
    glib_notify_reverse_rvv(b, static_cast<guint16>(len));

    // The dispatch call is outside the isolated loop in GLib, but its
    // element type, count, guard, and observed order must still agree.
    Dispatch da, db;
    if (len > 0) {
        dispatch_properties_changed(da, len, a);
        dispatch_properties_changed(db, len, b);
    }
    bool ok = scalar == rvv && da.calls == db.calls &&
              da.received == db.received &&
              da.calls == static_cast<unsigned>(len > 0);
    for (unsigned i = 0; i < len; ++i)
        ok &= a[i] == original[guard + len - 1 - i];
    for (unsigned i = 0; i < guard; ++i)
        ok &= scalar[i] == original[i] &&
              scalar[guard + len + i] == original[guard + len + i];
    if (!ok) {
        std::fprintf(stderr, "seed=0x12390b len=%u mode=%u case=%u\n", len,
                     mode, case_no);
        std::exit(1);
    }
}

int main() {
    constexpr unsigned seed = 0x12390bu;
    std::mt19937 rng(seed);
    glib_notify_reverse(nullptr, 0);
    glib_notify_reverse_rvv(nullptr, 0);
    const unsigned lengths[] = {
        0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 17, // tiny and odd/even
        31, 32, 33, 63, 64, 65, 127, 128, 129, // VLEN boundaries
        255, 256, 257, 511, 512, 513, 1023, // staging boundaries
        4095, 4096, 65534, 65535 // long and guint16 maximum
    };
    unsigned cases = 0;
    for (unsigned len : lengths)
        for (unsigned mode = 0; mode < 5; ++mode)
            check(len, mode, rng, ++cases);
    for (unsigned i = 0; i < 200; ++i)
        check(rng() % 1024, i % 5, rng, ++cases);
    std::printf("glib notify reverse: %u exact cases (seed 0x%x)\n", cases,
                seed);
}
