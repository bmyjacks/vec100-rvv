# VEC100-RVV

For redistribution and the licenses of the extracted kernels, see [LICENSE.md](LICENSE.md).

Results use `-O3` for RISC-V `rv64gcv` (LLVM 23.1.1 and GCC 16.1.0).
**Ordinary** uses the default vectorization settings; **aggressive** adds
`#pragma clang loop vectorize(enable)` to eligible loops for LLVM or
`-fvect-cost-model=unlimited` for GCC. See [BENCH.md](BENCH.md) for how
vectorization is measured.

In the per-kernel table, **LOOP** means a successful loop-vectorizer report,
**SLP** a successful SLP-vectorizer report, **LOOP+SLP** both, and **No** that
the vectorization criterion was not met. A positive result also requires
emitted RVV instructions in the compiled scalar source; it may come from a
helper rather than the intended hot loop.
**Difficulty** (0–5) is a qualitative estimate of the effort to obtain the
hand-written RVV approach through auto-vectorization, not a performance score.
**L / S** denotes loads / stores; **—** denotes no tagged RVV extension.

| Compiler | Ordinary | Aggressive |
|---|---:|---:|
| LLVM | 54/100 | 57/100 |
| GCC | 47/100 | 73/100 |

Each compiler column in the following summaries shows vectorized kernels /
kernels in that group, counting LOOP, SLP, and LOOP+SLP as vectorized.

### Vectorization by difficulty

| Difficulty | Kernels | LLVM | GCC | LLVM aggressive | GCC aggressive |
|---|---:|---:|---:|---:|---:|
| 0 | 62 | 38/62 | 35/62 | 40/62 | 49/62 |
| 1 | 8 | 4/8 | 3/8 | 4/8 | 5/8 |
| 2 | 12 | 5/12 | 5/12 | 6/12 | 8/12 |
| 3 | 7 | 1/7 | 1/7 | 1/7 | 4/7 |
| 4 | 6 | 4/6 | 2/6 | 4/6 | 4/6 |
| 5 | 5 | 2/5 | 1/5 | 2/5 | 3/5 |
| **Total** | **100** | **54/100** | **47/100** | **57/100** | **73/100** |

### Vectorization by application domain

| Application domain | Kernels | LLVM | GCC | LLVM aggressive | GCC aggressive |
|---|---:|---:|---:|---:|---:|
| color_processing | 1 | 1/1 | 0/1 | 1/1 | 0/1 |
| compression | 10 | 5/10 | 3/10 | 6/10 | 7/10 |
| cryptography | 7 | 4/7 | 2/7 | 4/7 | 7/7 |
| database | 1 | 0/1 | 0/1 | 0/1 | 0/1 |
| font_rendering | 12 | 8/12 | 6/12 | 8/12 | 9/12 |
| integer_arithmetic | 2 | 2/2 | 2/2 | 2/2 | 2/2 |
| json | 5 | 0/5 | 1/5 | 1/5 | 4/5 |
| machine_learning | 11 | 10/11 | 6/11 | 10/11 | 8/11 |
| multimedia | 23 | 16/23 | 16/23 | 17/23 | 18/23 |
| numerical | 11 | 7/11 | 6/11 | 7/11 | 9/11 |
| serialization | 3 | 0/3 | 1/3 | 0/3 | 1/3 |
| system_library | 5 | 0/5 | 1/5 | 0/5 | 1/5 |
| text_regex | 5 | 0/5 | 1/5 | 0/5 | 3/5 |
| unicode | 4 | 1/4 | 2/4 | 1/4 | 4/4 |
| **Total** | **100** | **54/100** | **47/100** | **57/100** | **73/100** |

### Vectorization by data dependence class

| Data dependence class | Kernels | LLVM | GCC | LLVM aggressive | GCC aggressive |
|---|---:|---:|---:|---:|---:|
| known_distance | 2 | 1/2 | 1/2 | 1/2 | 1/2 |
| none | 26 | 7/26 | 11/26 | 9/26 | 20/26 |
| potential_alias | 22 | 15/22 | 12/22 | 16/22 | 18/22 |
| recurrence | 16 | 7/16 | 4/16 | 7/16 | 10/16 |
| reduction | 29 | 21/29 | 16/29 | 21/29 | 20/29 |
| runtime_dependent | 5 | 3/5 | 3/5 | 3/5 | 4/5 |
| **Total** | **100** | **54/100** | **47/100** | **57/100** | **73/100** |

| Kernel | Difficulty | Application domain | Data dependence class | Computation | Memory access (L / S) | Control flow | Data types | Operation classes | Loop (depth / level / trip count) | RVV extensions | LLVM | GCC | LLVM aggressive | GCC aggressive |
|---|:---:|---|---|---|---|---|---|---|---|---|:---:|:---:|:---:|:---:|
| `00__xdg_mime_magic_matchlet_compare_to_data` | 0 | system_library | none | search | L: contiguous, broadcast<br>S: none | nested_control (early exit) | u8 | comparison, bitwise | 2 / innermost / runtime_invariant | — | No | No | No | No |
| `01_apply_deltas_to_points` | 2 | font_rendering | recurrence | map (search) | L: segmented, indexed<br>S: segmented, indexed | early_exit | f32, u8 | floating_point_arithmetic, comparison, min_max, division | 3 / innermost / data_dependent | — | LOOP | LOOP | LOOP | LOOP |
| `02_bli_zzpackm_1er_generic_ref` | 0 | numerical | potential_alias | map (multi_output) | L: segmented, broadcast<br>S: contiguous, strided | straight_line | f64 | floating_point_arithmetic, multiply_accumulate | 3 / outer / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `03_camellia_rounds` | 0 | cryptography | recurrence | recurrence | L: contiguous, indexed<br>S: contiguous | straight_line | u32 | bitwise, shift, rotate, lookup | 1 / single_loop / runtime_invariant | Zvbb | SLP | No | SLP | SLP |
| `04_caxpy_k` | 0 | numerical | potential_alias | map | L: segmented, strided, broadcast<br>S: segmented, strided | straight_line | f32 | floating_point_arithmetic, multiply_accumulate | 1 / single_loop / runtime_invariant | — | No | No | No | LOOP |
| `05_chacha20_encrypt_bytes` | 3 | cryptography | recurrence | recurrence | L: none<br>S: none | straight_line | u32 | integer_arithmetic, bitwise, shift, rotate | 2 / single_loop / compile_time_constant (short) | Zvbb | LOOP+SLP | LOOP | LOOP+SLP | LOOP+SLP |
| `06_copy_and_encode` | 4 | compression | recurrence | recurrence | L: contiguous, indexed<br>S: contiguous, indexed | straight_line | u8 | integer_arithmetic, bitwise | 1 / multiple / runtime_invariant | — | LOOP | No | LOOP | No |
| `07_countvarintsassuminglargearray` | 0 | serialization | reduction | reduction | L: contiguous<br>S: none | straight_line | u8, u64 | population_count, bitwise, integer_arithmetic | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `08_cvt_f32_f8e4m3_ref` | 5 | machine_learning | potential_alias | map | L: contiguous, broadcast<br>S: contiguous | multi_branch | f32, u8 | conversion, floating_point_arithmetic, transcendental, comparison, absolute_value, bitwise, shift, integer_arithmetic | 1 / single_loop / runtime_invariant | other | No | No | No | No |
| `09_cvt_f8e4m3_bf16_ref` | 0 | machine_learning | potential_alias | map | L: contiguous<br>S: contiguous | multi_branch | u8, f32, bf16 | conversion, bitwise, shift, comparison, integer_arithmetic | 1 / single_loop / runtime_invariant | Zvfhmin, other | LOOP | No | LOOP | SLP |
| `10_decompile_deltas_add_to_points` | 0 | font_rendering | potential_alias | map | L: contiguous, segmented, broadcast<br>S: segmented | switch | f32, i8, i16, i32 | floating_point_arithmetic, integer_arithmetic, conversion | 2 / multiple / data_dependent | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `11_decompile_points` | 3 | font_rendering | reduction | scan | L: contiguous<br>S: contiguous | if_else | u32, u16, u8 | integer_arithmetic, conversion | 2 / multiple / data_dependent | — | No | No | No | No |
| `12_decompose` | 0 | unicode | none | search | L: contiguous, indexed, broadcast<br>S: none | multi_branch (early exit) | u16, i32 | comparison, lookup, integer_arithmetic, shift, bitwise | 2 / innermost / data_dependent | — | No | No | No | LOOP+SLP |
| `13_dequantize_row_q4_k` | 0 | machine_learning | potential_alias | map (multi_output) | L: contiguous, indexed, broadcast<br>S: contiguous | straight_line | f16, u8, f32 | integer_arithmetic, bitwise, floating_point_arithmetic, conversion, lookup | 3 / multiple / compile_time_constant (medium) | Zvfhmin | SLP | LOOP | SLP | LOOP |
| `14_edge_sweep_row` | 3 | font_rendering | recurrence | recurrence (map) | L: contiguous, broadcast<br>S: contiguous | single_if | i16, i32 | integer_arithmetic, multiply_accumulate, comparison, min_max | 1 / multiple / runtime_invariant | — | No | No | No | SLP |
| `15_edgefilter_gaussian5x5` | 0 | multimedia | potential_alias | stencil | L: contiguous, strided<br>S: contiguous, strided | single_if | u8, i32 | integer_arithmetic, multiply_accumulate, division | 2 / innermost / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `16_encode_mcu_ac_first_prepare` | 0 | compression | reduction | reduction (map) | L: contiguous, indexed<br>S: contiguous | continue | i16, u16, i32, u64 | integer_arithmetic, bitwise, shift, comparison, absolute_value | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `17_estimatecupropagatecost` | 0 | multimedia | potential_alias | map | L: contiguous, broadcast<br>S: contiguous | straight_line | u16, i32, f64 | floating_point_arithmetic, integer_arithmetic, min_max, bitwise, division, conversion | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `18_fastcover_computefrequency` | 4 | compression | runtime_dependent | histogram | L: contiguous, strided, indexed<br>S: indexed | data_dependent_loop | u8, u32, u64 | integer_arithmetic, bitwise, shift | 2 / innermost / data_dependent | — | No | No | No | No |
| `19_fe_mul` | 5 | cryptography | none | map (multi_output) | L: contiguous<br>S: contiguous | straight_line | i32, i64 | integer_arithmetic, multiply_accumulate, shift, bitwise | 0 / unknown / unknown | — | No | No | No | SLP |
| `20_ffmpeg_emulated_edge_mc_16` | 0 | multimedia | potential_alias | map | L: contiguous, strided, broadcast<br>S: contiguous, strided | straight_line | u16 | other | 2 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `21_ffmpeg_yuv2planex_10` | 0 | multimedia | reduction | reduction (map) | L: contiguous, broadcast<br>S: contiguous | if_else | i16, i32, u16 | integer_arithmetic, multiply_accumulate, shift, min_max, conversion | 2 / outer / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `22_fft` | 0 | multimedia | none | permutation | L: strided<br>S: contiguous | straight_line | f32 | other | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `23_fillreferencesamples` | 0 | multimedia | recurrence | mixed | L: contiguous, strided, broadcast<br>S: contiguous | data_dependent_loop | u8, i32 | comparison, other | 2 / multiple / data_dependent | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `24_find_unordered_guint8` | 0 | serialization | known_distance | search | L: contiguous<br>S: none | early_exit | u8, u16, u32, u64 | comparison | 1 / multiple / runtime_invariant | — | No | No | No | No |
| `25_findposfirstlast_c` | 1 | multimedia | reduction | reduction (search) | L: constant, indexed<br>S: none | early_exit | i16, u16, u32 | integer_arithmetic, lookup | 1 / multiple / data_dependent | — | LOOP | LOOP | LOOP | LOOP |
| `26_ft_bitmap_embolden` | 1 | font_rendering | known_distance | stencil | L: contiguous, broadcast<br>S: contiguous | nested_control (early exit) | u8 | integer_arithmetic, bitwise, saturating_arithmetic | 3 / multiple / runtime_invariant | — | LOOP+SLP | LOOP+SLP | LOOP+SLP | LOOP+SLP |
| `27_ft_outline_get_bbox` | 0 | font_rendering | reduction | reduction | L: contiguous, segmented<br>S: none | single_if | i64, u8 | comparison, min_max | 1 / single_loop / runtime_invariant | — | LOOP+SLP | SLP | LOOP+SLP | SLP |
| `28_g_ascii_strdown` | 0 | system_library | none | map | L: contiguous, indexed<br>S: contiguous | straight_line | i8, u16 | integer_arithmetic, bitwise, comparison, lookup | 1 / multiple / data_dependent | — | No | No | No | No |
| `29_g_strreverse` | 0 | system_library | none | permutation | L: contiguous<br>S: contiguous | straight_line | i8 | other | 1 / single_loop / runtime_invariant | — | No | LOOP | No | LOOP |
| `30_gb_add_full_32` | 1 | numerical | runtime_dependent | map | L: contiguous, indexed<br>S: indexed | straight_line | f64, i64 | floating_point_arithmetic | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `31_gb_bitmap_subref` | 0 | numerical | none | permutation | L: contiguous, indexed, strided, broadcast<br>S: contiguous | multi_branch | u8, u32, u64, i64 | lookup | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `32_gb_bld_template` | 0 | numerical | potential_alias | permutation | L: contiguous, indexed<br>S: contiguous | straight_line | f64, i64 | lookup | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `33_gb_emult_02a` | 1 | numerical | recurrence | map (permutation) | L: contiguous, indexed<br>S: indexed | continue | f64, i64, i8 | floating_point_arithmetic | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `34_gb_select_phase2_entry` | 1 | numerical | recurrence | map (permutation) | L: contiguous, broadcast<br>S: indexed | single_if | f64, i64 | comparison | 1 / single_loop / runtime_invariant | — | No | No | No | SLP |
| `35_gb_select_phase2_tril` | 0 | numerical | potential_alias | map | L: contiguous<br>S: contiguous | straight_line | i64, f64 | other | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `36_gcm_ghash_4bit` | 0 | cryptography | recurrence | recurrence | L: contiguous, indexed, broadcast<br>S: contiguous | nested_control | u8, u64 | bitwise, shift, integer_arithmetic, lookup | 2 / innermost / compile_time_constant (short) | Zvbc | No | SLP | No | SLP |
| `37_gemm_f32rcprc_f32rcprc_f32rcprc_ref` | 0 | numerical | reduction | reduction (map) | L: strided, broadcast<br>S: strided | straight_line | f32 | floating_point_arithmetic, multiply_accumulate | 6 / innermost / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `38_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref` | 0 | machine_learning | reduction | reduction (map) | L: strided, broadcast<br>S: strided | multi_branch | u8, f32 | floating_point_arithmetic, multiply_accumulate, conversion, bitwise, shift, comparison, integer_arithmetic | 3 / innermost / runtime_invariant | other | LOOP | No | LOOP | SLP |
| `39_ggml_compute_forward_group_norm_f32` | 0 | machine_learning | reduction | reduction (map) | L: contiguous<br>S: contiguous | straight_line | f32, f64 | floating_point_arithmetic, multiply_accumulate, sqrt | 5 / multiple / runtime_invariant | — | LOOP | LOOP+SLP | LOOP | LOOP+SLP |
| `40_ggml_vec_dot_bf16` | 0 | machine_learning | reduction | reduction | L: contiguous<br>S: none | straight_line | bf16, f32, f64 | floating_point_arithmetic, multiply_accumulate, conversion, shift | 1 / single_loop / runtime_invariant | Zvfhmin | LOOP | LOOP | LOOP | LOOP |
| `41_ggml_vec_dot_f16` | 0 | machine_learning | reduction | reduction | L: contiguous, indexed, constant<br>S: none | straight_line | f16, f32, f64 | floating_point_arithmetic, multiply_accumulate, conversion, lookup | 1 / single_loop / runtime_invariant | Zvfh, Zvfhmin | LOOP | LOOP | LOOP | LOOP |
| `42_glib_notify_reverse` | 0 | system_library | none | permutation | L: contiguous<br>S: contiguous | straight_line | pointer | other | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `43_glib_str_hash` | 3 | system_library | recurrence | recurrence | L: contiguous<br>S: none | data_dependent_loop | i8, u32 | integer_arithmetic, shift | 1 / multiple / data_dependent | — | No | No | No | No |
| `44_gmp_primesieve` | 4 | integer_arithmetic | runtime_dependent | map (reduction) | L: contiguous, strided, constant<br>S: strided | nested_control (early exit) | u64 | bitwise, shift, integer_arithmetic, comparison | 2 / multiple / data_dependent | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `45_h2v2_fancy_upsample` | 2 | compression | recurrence | stencil | L: contiguous<br>S: contiguous | straight_line | u8, i32 | integer_arithmetic, shift, multiply_accumulate | 3 / innermost / runtime_invariant | — | LOOP | No | LOOP | SLP |
| `46_hb_iup_segment` | 1 | font_rendering | potential_alias | map | L: segmented, contiguous, broadcast<br>S: contiguous | multi_branch (early exit) | f64, f32, i32 | floating_point_arithmetic, comparison, division, conversion | 2 / innermost / runtime_invariant | — | LOOP | No | LOOP | LOOP |
| `47_hb_linear_gradient_region` | 1 | font_rendering | recurrence | map (recurrence) | L: contiguous, indexed, broadcast<br>S: contiguous | multi_branch | f32, u32, u8 | floating_point_arithmetic, integer_arithmetic, lookup, bitwise, shift, conversion, comparison, min_max, absolute_value | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `48_hb_set_masks` | 0 | font_rendering | none | map | L: segmented, broadcast<br>S: segmented | single_if | u32 | bitwise, comparison | 1 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `49_hist_count_simple` | 5 | compression | runtime_dependent | histogram (reduction) | L: contiguous, indexed<br>S: indexed | straight_line | u8, u32 | integer_arithmetic | 1 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `50_ia64_code` | 2 | compression | none | map | L: contiguous, indexed<br>S: contiguous | nested_control | u8, u32, u64 | integer_arithmetic, bitwise, shift, comparison, lookup | 3 / outer / runtime_invariant | — | No | No | LOOP | SLP |
| `51_icu_latin1_offsets` | 0 | unicode | potential_alias | map | L: contiguous, broadcast<br>S: contiguous | early_exit | u16, u8, i32 | comparison, bitwise, integer_arithmetic, conversion | 1 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `52_interp_hv_pp_8_16x16` | 0 | multimedia | potential_alias | stencil | L: contiguous, strided, broadcast<br>S: contiguous | straight_line | u8, i16, i32 | integer_arithmetic, multiply_accumulate, shift, min_max | 2 / multiple / compile_time_constant (short) | — | LOOP | LOOP | LOOP | LOOP |
| `53_interp_vert_pp_8_16x16` | 0 | multimedia | potential_alias | map (stencil) | L: strided, broadcast<br>S: contiguous | multi_branch | u8, i16, i32 | integer_arithmetic, multiply_accumulate, shift, min_max, saturating_arithmetic, conversion | 2 / innermost / compile_time_constant (short) | — | LOOP+SLP | LOOP | LOOP+SLP | LOOP |
| `54_intra_ang_horizontal_transpose` | 2 | multimedia | none | permutation | L: contiguous, strided<br>S: contiguous, strided | straight_line | u8 | other | 2 / innermost / compile_time_constant (short) | — | No | LOOP+SLP | No | LOOP+SLP |
| `55_jpeg_ac_refine` | 0 | compression | reduction | reduction (map) | L: contiguous, indexed<br>S: contiguous | multi_branch | i16, u16, i32, u64 | integer_arithmetic, bitwise, shift, comparison, absolute_value | 1 / single_loop / runtime_invariant | — | No | No | No | SLP |
| `56_jpeg_int_downsample` | 0 | compression | reduction | reduction (map) | L: contiguous<br>S: contiguous | straight_line | u8, i64 | integer_arithmetic, division, conversion | 4 / innermost / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `57_lh_table_new` | 0 | json | none | map | L: none<br>S: strided | straight_line | pointer | other | 1 / single_loop / runtime_invariant | — | No | No | LOOP | LOOP+SLP |
| `58_llama_quantize_q8_0` | 0 | machine_learning | reduction | reduction (map) | L: contiguous<br>S: contiguous | straight_line | f32, i8, f16 | floating_point_arithmetic, absolute_value, min_max, division, conversion | 2 / multiple / compile_time_constant (medium) | Zvfhmin | LOOP | No | LOOP | No |
| `59_lz4_renorm_dict_t` | 0 | compression | none | map | L: contiguous<br>S: contiguous | if_else | u32 | comparison, integer_arithmetic | 1 / single_loop / compile_time_constant (long) | — | LOOP | LOOP | LOOP | LOOP |
| `60_minify` | 5 | json | recurrence | map (permutation) | L: contiguous, indexed<br>S: indexed | data_dependent_loop | u8 | lookup, bitwise, integer_arithmetic | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `61_mpn_fft_mul_2exp_modf` | 4 | integer_arithmetic | potential_alias | recurrence (map) | L: contiguous, broadcast<br>S: contiguous | straight_line | u64 | integer_arithmetic, bitwise, shift, comparison | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `62_normalize_glyphs_cluster` | 2 | font_rendering | reduction | scan (reduction) | L: segmented<br>S: segmented | straight_line | i32 | integer_arithmetic, saturating_arithmetic | 1 / multiple / runtime_invariant | — | LOOP | SLP | LOOP | SLP |
| `63_number_of_digits_decimal_left_shift` | 0 | json | none | search | L: contiguous, constant<br>S: none | multi_branch (early exit) | u8 | comparison, lookup, shift, bitwise, integer_arithmetic | 1 / single_loop / data_dependent | — | No | LOOP | No | LOOP+SLP |
| `64_ossl_base64_decode` | 3 | cryptography | potential_alias | map | L: contiguous, indexed<br>S: contiguous | single_if (early exit) | u8, u64 | lookup, bitwise, shift | 1 / single_loop / runtime_invariant | — | No | No | No | SLP |
| `65_ossl_sm4_encrypt` | 5 | cryptography | recurrence | recurrence | L: indexed, constant<br>S: contiguous | straight_line | u32 | bitwise, shift, rotate, lookup | 0 / unknown / unknown | Zvksed | SLP | No | SLP | SLP |
| `66_partialbutterfly8` | 0 | multimedia | potential_alias | map (multi_output) | L: contiguous, constant<br>S: strided | straight_line | i16, i32 | integer_arithmetic, multiply_accumulate, shift, conversion | 2 / outer / runtime_invariant | — | LOOP | No | LOOP | SLP |
| `67_pcre2_class_repeat` | 0 | text_regex | none | search | L: contiguous, indexed<br>S: none | switch (early exit) | u8 | comparison, bitwise, lookup | 1 / multiple / data_dependent | — | No | No | No | SLP |
| `68_pcre2_compare_opcodes` | 0 | text_regex | none | search | L: contiguous<br>S: none | single_if (early exit) | u8 | bitwise, comparison | 1 / multiple / compile_time_constant (medium) | — | No | LOOP | No | LOOP+SLP |
| `69_pcre2_match` | 0 | text_regex | none | search | L: contiguous, indexed<br>S: none | single_if (early exit) | u8 | bitwise, comparison, shift | 1 / multiple / data_dependent | — | No | No | No | No |
| `70_put_pixels8_8_c` | 0 | multimedia | potential_alias | map | L: contiguous, strided<br>S: contiguous, strided | straight_line | u8, u32 | other | 1 / single_loop / runtime_invariant | — | No | No | No | LOOP |
| `71_quant_c` | 0 | multimedia | reduction | reduction (map) | L: contiguous<br>S: contiguous | single_if | i16, i32, u32 | integer_arithmetic, multiply_accumulate, absolute_value, comparison, shift, min_max, saturating_arithmetic, conversion | 1 / single_loop / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `72_quantize_row_q4_0_ref` | 0 | machine_learning | reduction | reduction (map) | L: contiguous<br>S: contiguous | multi_branch | f32, i8, u8, f16 | floating_point_arithmetic, absolute_value, comparison, min_max, conversion, saturating_arithmetic, bitwise, shift | 2 / multiple / compile_time_constant (medium) | Zvfhmin | LOOP | No | LOOP | No |
| `73_quantize_row_q8_k_ref` | 0 | machine_learning | reduction | reduction (map) | L: contiguous<br>S: contiguous | continue | f32, i8, i16, i32 | floating_point_arithmetic, absolute_value, min_max, division, conversion, integer_arithmetic | 3 / multiple / compile_time_constant (long) | — | SLP | LOOP | SLP | LOOP |
| `74_read_points` | 4 | font_rendering | reduction | scan | L: contiguous, segmented<br>S: segmented | if_else (early exit) | f32, i16, u8 | integer_arithmetic, bitwise, comparison | 1 / single_loop / runtime_invariant | — | LOOP | No | LOOP | LOOP |
| `75_satd_4x4` | 2 | multimedia | reduction | reduction (map) | L: contiguous<br>S: none | straight_line | u8, i16, i32 | integer_arithmetic, bitwise, shift, absolute_value | 1 / multiple / compile_time_constant (short) | — | SLP | LOOP | SLP | LOOP |
| `76_scan_for_newline` | 4 | text_regex | none | search | L: contiguous, broadcast<br>S: none | switch (early exit) | i8 | comparison | 1 / multiple / runtime_invariant | — | No | No | No | SLP |
| `77_simdjson_eight_digits` | 0 | json | none | map (reduction) | L: contiguous<br>S: none | straight_line | u8, u64 | comparison, bitwise, shift, integer_arithmetic | 0 / unknown / unknown | — | No | No | No | SLP |
| `78_skl_softmax_f32` | 0 | machine_learning | reduction | mixed (reduction, map) | L: contiguous, broadcast<br>S: contiguous | straight_line | f32 | floating_point_arithmetic, transcendental, comparison, min_max, division | 1 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `79_slopelimiting` | 0 | color_processing | none | map | L: none<br>S: contiguous | straight_line | u16, f64 | floating_point_arithmetic, saturating_arithmetic, conversion | 1 / multiple / runtime_invariant | — | LOOP | No | LOOP | No |
| `80_sqlite_to_base85` | 2 | database | recurrence | map | L: contiguous<br>S: contiguous | single_if | u8, u32 | integer_arithmetic, division, shift, bitwise | 2 / outer / runtime_invariant | — | No | No | No | No |
| `81_sscal_k` | 0 | numerical | none | map | L: strided, broadcast<br>S: strided | if_else | f32 | floating_point_arithmetic, comparison | 1 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `82_suitesparse_cholmod_norm_dense` | 0 | numerical | reduction | reduction | L: contiguous<br>S: none | straight_line | f64 | floating_point_arithmetic, absolute_value, comparison, min_max | 2 / innermost / runtime_invariant | — | LOOP | No | LOOP | LOOP |
| `83_sweep_row_to_alpha` | 3 | font_rendering | reduction | scan (map) | L: contiguous<br>S: contiguous | single_if | u8, i16, i32 | integer_arithmetic, comparison, absolute_value, shift | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `84_transpose_16` | 0 | multimedia | potential_alias | permutation | L: strided<br>S: contiguous | straight_line | u8 | other | 2 / innermost / compile_time_constant (short) | — | No | No | LOOP | No |
| `85_u_strfromutf8withsub` | 2 | unicode | recurrence | map | L: contiguous, indexed, broadcast<br>S: contiguous | multi_branch (early exit) | u8, u16, i32 | integer_arithmetic, bitwise, shift, comparison, lookup, conversion | 2 / innermost / runtime_invariant | — | No | No | No | SLP |
| `86_update_classbits` | 2 | text_regex | reduction | map | L: indexed, broadcast<br>S: contiguous | switch | u8, u16, u32 | comparison, bitwise, shift, lookup | 1 / single_loop / compile_time_constant (long) | — | No | No | No | No |
| `87_utf8_range_validateutf8naive` | 0 | serialization | reduction | search | L: contiguous<br>S: none | multi_branch (early exit) | u8 | comparison, integer_arithmetic | 1 / single_loop / data_dependent | — | No | LOOP | No | LOOP |
| `88_utf8_verify_ascii` | 0 | unicode | none | search (map) | L: contiguous, broadcast<br>S: none | early_exit | u8 | comparison, bitwise, integer_arithmetic | 2 / innermost / data_dependent | — | No | LOOP | No | LOOP+SLP |
| `89_validate_utf8` | 0 | json | none | search (map) | L: contiguous<br>S: none | early_exit | u8, u32, u64 | comparison, bitwise, shift, integer_arithmetic | 1 / single_loop / data_dependent | — | No | No | No | SLP |
| `90_whisper_mel_filterbank` | 2 | multimedia | reduction | reduction (map) | L: contiguous<br>S: strided | straight_line | f32, f64 | floating_point_arithmetic, multiply_accumulate, transcendental, conversion | 2 / innermost / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP+SLP |
| `91_x265_blockfill_s_16` | 0 | multimedia | none | map | L: broadcast<br>S: contiguous, strided | straight_line | i16 | other | 2 / innermost / compile_time_constant (short) | — | SLP | LOOP | SLP | LOOP |
| `92_x265_pelfilter_luma` | 0 | multimedia | none | stencil (multi_output) | L: strided, broadcast<br>S: strided | nested_control | u8, i16, i32 | integer_arithmetic, comparison, absolute_value, bitwise, shift, min_max | 1 / single_loop / compile_time_constant (short) | — | No | LOOP | No | LOOP+SLP |
| `93_x265_plane_statistics` | 0 | multimedia | reduction | reduction | L: contiguous, strided<br>S: none | straight_line | u8, u64 | integer_arithmetic, min_max | 2 / multiple / runtime_invariant | — | LOOP | No | LOOP | No |
| `94_x265_process_sao_cue1` | 1 | multimedia | potential_alias | map | L: contiguous, strided, indexed<br>S: contiguous, strided | straight_line | u8, i8, i32 | integer_arithmetic, bitwise, shift, min_max, lookup | 2 / innermost / runtime_invariant | — | No | No | No | No |
| `95_x265_rdo_recount_sign` | 2 | multimedia | reduction | reduction (map) | L: constant, indexed<br>S: indexed | straight_line | i16, u16, i32, u32 | integer_arithmetic, bitwise, shift, comparison | 1 / single_loop / runtime_invariant | — | No | No | No | No |
| `96_x265_ssim_distortion` | 0 | multimedia | reduction | reduction (map) | L: strided<br>S: none | straight_line | u8, i32, u32, u64 | integer_arithmetic, multiply_accumulate, shift, conversion | 2 / multiple / runtime_invariant | — | LOOP | LOOP | LOOP | LOOP |
| `97_xtime` | 0 | cryptography | none | map | L: contiguous<br>S: contiguous | straight_line | u8, u32 | integer_arithmetic, bitwise, shift | 1 / multiple / compile_time_constant (short) | Zvkned, Zvbc | SLP | No | SLP | SLP |
| `98_yuv2rgb_c_24_rgb` | 2 | multimedia | potential_alias | map | L: contiguous, indexed, indirect<br>S: segmented | straight_line | u8, i32 | integer_arithmetic, lookup | 2 / innermost / runtime_invariant | — | No | No | No | No |
| `99_zstd_buildfsetable_body` | 3 | compression | runtime_dependent | map (histogram) | L: segmented, indexed<br>S: segmented, indexed | straight_line | u8, u16, u32 | integer_arithmetic, bitwise, shift, lookup | 1 / single_loop / runtime_invariant | — | No | No | No | LOOP+SLP |
