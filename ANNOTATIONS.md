# Kernel annotation glossary

This file explains the annotation columns in [README.md](README.md). The
source-level tags describe the scalar code as written, not whether a compiler
will vectorize it or how fast it runs. **Difficulty** is a separate qualitative
comparison with the hand-written RVV implementation; compiler results are
experimental measurements.

The lists below cover the values used in the current 100-kernel table.
`unknown` in the loop annotations means the supplied source or
metadata does not establish the property; it does not mean `none`.

## Data dependence class

This column corresponds to `dependence.pattern`: how work in different
iterations is related, or what prevents independence from being established.
A **loop-carried dependence** means an iteration uses state produced by an
earlier iteration.

| Value | Meaning | Illustrative scalar code |
|---|---|---|
| `none` | Iterations appear independent from source inspection. This is not a claim that vectorization succeeds. | `out[i] = in[i] * 2`, with independence established. |
| `reduction` | Iterations contribute to one or a few accumulated results, such as a sum, minimum, maximum, or combined bit mask. | `sum += a[i]` |
| `known_distance` | A dependence has a statically identifiable iteration distance. | `a[i] = a[i - 2] + x[i]`: distance two. |
| `recurrence` | A general loop-carried state dependency that is not represented as a reduction. | `state = f(state, a[i])` |
| `potential_alias` | Independence or vectorization legality depends on whether pointers overlap, and the source does not prove that they cannot. Do not assume `restrict`. | `out[i] = in[i] + 1`, with unconstrained pointer overlap. |
| `runtime_dependent` | Access relationships or conflicts depend on runtime values, such as indices or offsets. Different iterations may update the same location. | `a[index[i]] += b[i]` |

Important distinctions:

- `potential_alias` concerns overlap between memory regions; `runtime_dependent`
  concerns relationships determined by runtime addresses/indices, including
  repeated histogram bins even when the input and output buffers are disjoint.
- `known_distance` is more specific than a general `recurrence`; both can
  describe related dependency structures. The annotations also use
  `known_distance` for a carried previous-element comparison in
  `24_find_unordered_guint8`. That tag does not necessarily imply an in-place
  read-after-write dependency or prohibit a vector reformulation.
- A `reduction` is a special accumulated dependency, not a general state
  recurrence. Floating-point reductions still have ordering/rounding constraints.
- An indexed **read** alone does not imply `runtime_dependent`: independent
  gathers can have dependence `none` (or feed a `reduction`). Conflicting
  indexed updates are the key issue in the example above.
- **Computation** describes what the kernel does; **data dependence** describes
  relationships in its implementation. The two columns need not match: a
  `map` can have `potential_alias`, and a `scan` can be tagged `reduction` in
  the annotations.

## Difficulty

Values range from **0 to 5**, from lower to higher estimated effort to obtain
the particular manual RVV approach through auto-vectorization of the scalar
source as written. They are not measured success rates, speedup scores, or a
strict ordering within each level. Level 0 does not guarantee compiler success.

| Value | Meaning |
|---|---|
| `0` | Same essential algorithm; lane-wise work, masks, searches, ordinary reductions, or scalar fallback. |
| `1` | Local vector idiom or legality guard; a borderline implementation change. |
| `2` | Local representation change, specialization, or guarded batching. |
| `3` | Reorganize an ordered recurrence, or reorder validation while preserving observable behavior. |
| `4` | Dependency-safe batching, irregular updates, or discovery of safe regions in stateful processing. |
| `5` | Domain-specific mathematical/table substitution, state-machine decomposition, or reversal of the work/iteration space. |

## Application domain

`source.domain` describes the kernel's application context, not its arithmetic
or vectorization behavior.

| Value | Meaning |
|---|---|
| `color_processing` | Color conversion, adjustment, or color-management processing. |
| `compression` | Compression/decompression and related encoding or decoding. |
| `cryptography` | Ciphers, cryptographic hashes, or related cryptographic computation. |
| `database` | Database-related processing. |
| `font_rendering` | Glyph, outline, bitmap, or font-layout processing. |
| `integer_arithmetic` | Integer/multiprecision arithmetic and related number-theoretic routines. |
| `json` | JSON parsing, validation, formatting, or related utilities. |
| `machine_learning` | Machine-learning tensor operations, normalization, or quantization. |
| `multimedia` | Image, video, audio, or media-codec processing. |
| `numerical` | Numerical linear algebra and other numerical computation. |
| `serialization` | Encoding, decoding, or validation of serialized data. |
| `system_library` | General-purpose system/library utilities, such as strings or containers. |
| `text_regex` | Text searching or regular-expression processing. |
| `unicode` | Unicode encoding, decoding, validation, or character processing. |

## Computation

The first value is `computation.primary_pattern`; values in parentheses are
`secondary_patterns`. For example, `reduction (map)` denotes a primary reduction
with additional element-wise processing.

| Value | Meaning |
|---|---|
| `map` | Element-wise computation, conceptually independent per element. |
| `reduction` | Many elements contribute to one or a small number of results. |
| `scan` | Prefix-like computation producing intermediate accumulated outputs, e.g. `out[i] = out[i - 1] + a[i]`. |
| `search` | Find an element or condition, often with first-match/early-exit semantics. |
| `stencil` | Each output depends on neighboring input elements. |
| `histogram` | Update bins selected by input data, e.g. `hist[a[i]]++`. |
| `permutation` | Rearrange or select elements, including shuffles, lookups, and transposes. |
| `recurrence` | Repeated computation uses a value produced by an earlier iteration. |
| `multi_output` | Each iteration computes multiple logically related outputs. |
| `mixed` | No single computation pattern reasonably represents the kernel; not merely multiple arithmetic operations. |

## Memory access (L / S)

**L** lists `memory.loads`; **S** lists `memory.stores`. Loads and stores are
classified independently, and multiple patterns can appear on either side.
These are source access patterns, not claims about emitted vector instructions.

| Value | Meaning |
|---|---|
| `contiguous` | Adjacent iterations access adjacent elements, e.g. `a[i]`. |
| `strided` | Successive addresses differ by a loop-invariant stride, e.g. `a[2 * i]` or `a[i * stride]`. |
| `segmented` | Interleaved/array-of-structures fields, e.g. `pixels[i].r`, `.g`, and `.b`. |
| `indexed` | An address uses a varying index: a load such as `a[index[i]]` represents a gather; a store such as `out[index[i]] = value` represents a scatter. Both use the `indexed` tag. |
| `indirect` | Pointer chasing or multiple lookup levels, e.g. `node[i]->value` or `a[index1[index2[i]]]`. |
| `broadcast` | One scalar value is reused across iterations/lanes, e.g. a scale factor. |
| `constant` | Access to constant data, such as a fixed coefficient or lookup table; not a synonym for contiguous access. |
| `none` | No relevant memory accesses in this direction; local/register computations may still exist. |

## Control flow

This is `control_flow.pattern`. A parenthetical **early exit** additionally
means `control_flow.early_exit` is true; it can accompany another pattern.

| Value | Meaning |
|---|---|
| `straight_line` | No control-flow decision inside the candidate loop body. |
| `single_if` | One conditional branch, without an alternative `else` computation. |
| `if_else` | Conditional execution with alternative true/false paths. |
| `multi_branch` | Multiple independent or chained branch conditions. |
| `switch` | A switch or jump-table-like structure. |
| `early_exit` | Data-dependent `break`, `return`, or exit to a label before normal loop completion. |
| `continue` | Iterations are conditionally skipped. |
| `nested_control` | Nested control-flow decisions. |
| `data_dependent_loop` | Loop continuation/count depends on values read during execution, e.g. scanning until a zero byte. |

## Data types

`data.element_types` lists significant computation element types, not incidental
loop counters. Multiple entries are listed separately.

| Value | Meaning |
|---|---|
| `i8`, `i16`, `i32`, `i64` | Signed integers of 8, 16, 32, or 64 bits. |
| `u8`, `u16`, `u32`, `u64` | Unsigned integers of 8, 16, 32, or 64 bits. |
| `f16` | IEEE binary16 (half-precision floating point). |
| `bf16` | Bfloat16: 16-bit floating point with an 8-bit exponent and 7 explicit fraction bits. |
| `f32` | IEEE binary32 (single precision). |
| `f64` | IEEE binary64 (double precision). |
| `pointer` | Pointer-valued elements participate in the computation. |

## Operation classes

`data.operation_classes` can contain multiple entries. These describe operations
in the source, not necessarily individual ISA instructions.

| Value | Meaning |
|---|---|
| `integer_arithmetic` | Integer arithmetic, such as addition, subtraction, or multiplication. |
| `floating_point_arithmetic` | Floating-point arithmetic. |
| `multiply_accumulate` | Products added to an accumulator, e.g. `sum += a[i] * b[i]`; does not imply fused execution. |
| `comparison` | Equality or ordering tests. |
| `min_max` | Minimum/maximum selection or clamping. |
| `bitwise` | Bit-level AND, OR, XOR, or NOT. |
| `shift` | Left/right bit shifts. |
| `rotate` | Bit rotations with wraparound. |
| `population_count` | Count the set bits in a value. |
| `conversion` | Change numeric type or representation. |
| `saturating_arithmetic` | Arithmetic/conversion clamped at representable or specified limits. |
| `absolute_value` | Compute a magnitude/absolute value. |
| `division` | Division-related computation. |
| `sqrt` | Square-root computation. |
| `transcendental` | Functions such as exponential, logarithm, or trigonometric functions. |
| `lookup` | Table-based value selection. |
| `other` | Operations outside the named classes, including some copy/rearrangement-only kernels. |

## Loop (depth / level / trip count)

The three parts are `loop.nesting_depth`, `loop.vector_candidate_level`, and
`loop.trip_count_kind`. A size, when known, appears in parentheses after the
trip-count kind. For example, `2 / innermost / compile_time_constant (short)`
means a two-level loop nest, an inner-loop candidate, and a fixed short count.

**Depth** is the syntactic nesting depth: `1` for a single loop, `2` for two
nested loops, etc. `0` indicates no syntactic loop, as in explicitly unrolled
code; it does not rule out SLP vectorization.

| Candidate level | Meaning |
|---|---|
| `single_loop` | A single loop is the candidate for repeated vector work. |
| `innermost` | The innermost loop in a nest is the candidate. |
| `outer` | An outer loop in a nest is the candidate. |
| `multiple` | Multiple loops or levels contain candidate work. |
| `unknown` | A candidate level cannot be established, including some loop-free kernels. |

| Trip-count kind | Meaning |
|---|---|
| `compile_time_constant` | The iteration count is fixed in the source, e.g. `i < 64`. |
| `runtime_invariant` | The bound is supplied at runtime but remains invariant during the loop, e.g. `i < n`. |
| `data_dependent` | The count/termination depends on data examined while executing the loop. |
| `unknown` | The count kind cannot be established. |

| Trip-count size | Recommended interpretation |
|---|---|
| `short` | Typically at most 16 iterations. |
| `medium` | Typically 17–128 iterations. |
| `long` | Typically more than 128 iterations. |

A size is omitted when no supported size information is available.
A runtime bound alone does not justify a size estimate. Trip-count kind and
early exit are separate annotations: a loop can have an invariant upper bound
and still exit early.

## RVV extensions

`rvv.possible_extensions` describes possible optional ISA relevance. It does
**not** assert that the compiler emits the extension, that every path requires
it, or that the benchmark build enables it. Base RVV relevance is separate.
**—** means the optional-extension list is empty, not that RVV is irrelevant.

| Value | Meaning |
|---|---|
| `Zvbb` | Vector bit-manipulation operations, such as rotations, bit reversal, and population count. |
| `Zvbc` | Vector carry-less multiplication, relevant to polynomial/Galois-field arithmetic. |
| `Zvfh` | Vector half-precision floating-point arithmetic. |
| `Zvfhmin` | Minimal vector half-precision support, principally conversion between half and single precision. |
| `Zvkned` | Vector AES encryption/decryption operations. |
| `Zvksed` | Vector SM4 encryption/decryption operations. |
| `other` | Potential extension relevance not covered by the named tags; not a specific ISA extension name. |

## Compiler result columns

These are measured outcomes rather than source tags; see [BENCH.md](BENCH.md).

| Value | Meaning |
|---|---|
| `LOOP` | A successful loop-vectorizer report, with emitted RVV instructions. |
| `SLP` | A successful superword-level parallelism (SLP) vectorizer report, with emitted RVV instructions. SLP groups similar scalar operations and need not require a loop. |
| `LOOP+SLP` | Both kinds of successful vectorizer report, with emitted RVV instructions. |
| `No` (Not vectorized) | The vectorization criterion was not met. |

Positive results may come from a helper rather than the intended hot loop.
**Default** uses the compiler's default vectorization settings; **aggressive**
adds `#pragma clang loop vectorize(enable)` to eligible loops for LLVM or
`-fvect-cost-model=unlimited` for GCC. Summary fractions count `LOOP`, `SLP`,
and `LOOP+SLP` as vectorized kernels; the denominator is the group size.
