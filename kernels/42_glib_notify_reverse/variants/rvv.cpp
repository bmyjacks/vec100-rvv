#include "kernel.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <riscv_vector.h>

static_assert(sizeof(GParamSpec *) == sizeof(std::uint64_t),
              "RV64 pointer width");

void glib_notify_reverse_rvv(GParamSpec **pspecs, guint16 len) {
    // RVV integer loads from a GParamSpec** would violate C++ type aliasing.
    // Copy object representations to integer staging arrays, vector-swap,
    // then copy them back without ever interpreting a pointer as an integer.
    alignas(8) std::uint64_t left[256];
    alignas(8) std::uint64_t right[256];
    const size_t pairs = static_cast<size_t>(len) / 2;
    size_t done = 0;
    while (done < pairs) {
        const size_t vl =
            __riscv_vsetvl_e64m1(std::min(pairs - done, size_t{256}));
        GParamSpec **lo = pspecs + done;
        GParamSpec **hi = pspecs + static_cast<size_t>(len) - done - vl;
        std::memcpy(left, lo, vl * sizeof(GParamSpec *));
        std::memcpy(right, hi, vl * sizeof(GParamSpec *));

        const vuint64m1_t a = __riscv_vle64_v_u64m1(left, vl);
        const vuint64m1_t b = __riscv_vlse64_v_u64m1(right + vl - 1, -8, vl);
        __riscv_vsse64_v_u64m1(right + vl - 1, -8, a, vl);
        __riscv_vse64_v_u64m1(left, b, vl);
        std::memcpy(lo, left, vl * sizeof(GParamSpec *));
        std::memcpy(hi, right, vl * sizeof(GParamSpec *));
        done += vl;
    }
}
