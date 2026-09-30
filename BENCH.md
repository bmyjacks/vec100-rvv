# Measure compiler auto-vectorization

After selecting a compiler and flags in the root `build.sh`, build the kernels
and measure them:

```bash
bash build.sh
python3 -B bench.py
```

The root `build.sh` enables compiler-specific vectorization remarks when a
package compiles `assembly.s`: each kernel saves `build/vectorization.txt`, and
Clang also saves `build/vectorization.yaml`. These reporting flags are applied
only to `-S` builds, not to the test executable. Build progress is printed to
the terminal; other compiler output is saved to each kernel's `build/build.log`.
Builds run with up to 128 concurrent kernels by default; set `BUILD_JOBS` to
override that limit.

The summary is printed to the terminal; per-kernel results go to
`bench-results.csv`. For a smaller run or a separate output file:

```bash
bash kernels/01_apply_deltas_to_points/build.sh
python3 -B bench.py --kernel 01_apply_deltas_to_points --output clang-results.csv
```

Rebuild after changing `build.sh` or switching compilers. `bench.py` reads the
selected compiler from the root `build.sh` and analyzes each kernel's existing
`build/assembly.s` and vectorization report in parallel (128 readers by
default; adjust with `--jobs`). It does not recompile kernels or modify their
build outputs. To change the compiler, edit `build.sh`, rebuild, and run
`bench.py` again (use `--output` to keep both CSV files).

A kernel counts as **vectorized** when the build's report records a successful
loop-vectorizer or SLP transformation and `build/assembly.s` contains RVV
instructions. Missing or stale build reports are errors, not negative results.
`loop` includes loop-only and loop+SLP kernels; `SLP-only` excludes kernels
also vectorized by the loop vectorizer. `no` (not vectorized) means this criterion was not met;
`error` (including missing or stale assembly) is excluded from the denominator.
The CSV records the compiler version, pass-site counts, RVV instruction counts,
and any errors.

This measures auto-vectorization of `src/kernel.cpp`, the source compiled to
`assembly.s`, rather than the hand-written `variants/rvv.cpp`. A positive result
can come from code outside the kernel's intended hot loop. When that distinction
matters, examine `kernels/<kernel_id>/build/vectorization.txt` (and, for Clang,
`vectorization.yaml` in the same directory) alongside
`kernels/<kernel_id>/build/assembly.s`.
