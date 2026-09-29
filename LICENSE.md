# Licenses and third-party notices

The 100 kernels contain extracted and adapted code from other projects alongside
benchmark-authored code. **This repository as a whole is not CC0 or public
domain.** The original and third-party portions have different terms.

## Benchmark-authored contributions

To the extent that the benchmark contributors own the relevant rights, they
dedicate their original contributions—including test harnesses, build scripts,
hand-written RVV variants, and original portions of extraction glue and
documentation—under [CC0 1.0 Universal](LICENSES/CC0-1.0.txt). CC0 includes a
license fallback where the public-domain waiver is ineffective. It **does not**
relicense upstream-derived code, including upstream material incorporated into
files that also contain benchmark-authored changes. Copyright holders can only
dedicate rights they own; confirm authorization from all benchmark contributors.

## Upstream-derived material

Retain the original notices and provenance in `kernels/<ID>*/src/kernel.cpp`
and `include/kernel.h` when redistributing the code. Some kernel source files
refer to their upstream license instead of reproducing it: applicable license
texts from the pinned upstream source trees are bundled in `LICENSES/`. The
numeric prefixes below identify **all 100 directories** under `kernels/`.
Terms apply to upstream-derived portions, not automatically to every line of
a kernel. Where upstream offered alternatives, the chosen terms for this
distribution are noted; other alternatives are not revoked.

| Upstream project | Kernel IDs | Terms and bundled text |
| --- | --- | --- |
| BLIS 2.1 | 02 | BSD-style; [license](LICENSES/blis/LICENSE) and retained notices |
| FFmpeg n9.0.2 | 20, 21, 70, 98 | LGPL-2.1-or-later; [text](LICENSES/ffmpeg/COPYING.LGPLv2.1) |
| FreeType 2.14.3 | 26, 27 | FreeType License chosen here; [license options](LICENSES/freetype/LICENSE.TXT) and [FTL text](LICENSES/freetype/docs/FTL.TXT). Attribution required by FTL |
| GLib 2.90.0 | 00, 24, 28, 29, 42, 43, 76, 88 | LGPL-2.1-or-later; [text](LICENSES/glib/COPYING). Kernel 00 also permits AFL-2.0 as an alternative, not used here |
| GNU MP (GMP) 6.3.0 | 44, 61 | [LGPL-3.0-or-later](LICENSES/gmp/COPYING.LESSERv3) chosen here (also incorporates [GPL-3](LICENSES/gmp/COPYINGv3)); upstream also offers [GPL-2.0-or-later](LICENSES/gmp/COPYINGv2) |
| HarfBuzz 14.4.0 | 01, 10, 11, 14, 46, 47, 48, 62, 74, 83 | Old MIT-style terms; [COPYING](LICENSES/harfbuzz/COPYING) and per-file retained notices |
| ICU4C 78.3 | 12, 51, 85 | [Unicode License v3](LICENSES/icu/LICENSE) and retained notices |
| json-c 0.19 | 57 | MIT; [COPYING](LICENSES/json-c/COPYING) |
| libjpeg-turbo 3.2.0 | 16, 45, 55, 56 | IJG terms; [README.ijg](LICENSES/libjpeg-turbo/README.ijg) and [license overview](LICENSES/libjpeg-turbo/LICENSE.md) |
| libsodium 1.0.22 | 05, 97 | 05 includes a public-domain ChaCha20 notice and helper code from libsodium; 97 has an ISC notice. [Project ISC license](LICENSES/libsodium/LICENSE) |
| Little CMS 2 (lcms2) 2.19.1 | 79 | MIT-style; [license](LICENSES/littlecms/LICENSE) |
| llama.cpp 0.4.1 / ggml | 13, 40, 58, 73 | MIT; [license](LICENSES/llama.cpp/LICENSE) and retained notices |
| LZ4 1.10.0 | 59 | BSD-2-Clause; [license](LICENSES/lz4/LICENSE) |
| OpenBLAS 0.3.34 | 04, 81 | BSD-style; [license](LICENSES/openblas/LICENSE) and retained notices |
| OpenSSL 4.0.2 | 03, 19, 36, 64, 65 | Apache-2.0; [upstream license](LICENSES/openssl/LICENSE.txt) |
| PCRE2 10.48 | 67, 68, 69, 86 | BSD-3-Clause with PCRE2 exception; [licence](LICENSES/pcre2/LICENCE.md) and retained notices |
| Protocol Buffers 36.2 | 07, 87 | 07: [BSD-style](LICENSES/protobuf/LICENSE); 87: [MIT-style utf8_range](LICENSES/protobuf/utf8_range-LICENSE) |
| SiFive Kernel Library 3.0.0 | 08, 09, 37, 38, 78 | MIT; [license](LICENSES/skl/LICENSE.txt) |
| simdjson 4.6.11 | 60, 63, 77, 89 | Apache-2.0; [text](LICENSES/Apache-2.0.txt). Kernel 77 is from an amalgamation |
| SQLite 3.53.4 | 80 | Upstream public-domain dedication; [license information](LICENSES/sqlite/LICENSE.md) and retained notice |
| stable-diffusion.cpp / ggml | 39, 72 | MIT; [ggml license](LICENSES/stable-diffusion.cpp/ggml-LICENSE) and retained notices |
| SuiteSparse:GraphBLAS 10.5.1 | 30, 31, 32, 33, 34, 35 | Apache-2.0; [project license](LICENSES/graphblas/LICENSE) and [full text](LICENSES/Apache-2.0.txt) |
| SuiteSparse CHOLMOD 7.14.1 | 82 | **Mixed:** GPL-2.0-or-later worker and Apache-2.0 headers (per-file SPDX); [SuiteSparse notices](LICENSES/suitesparse/LICENSE.txt), [GPL-2](LICENSES/gmp/COPYINGv2), [Apache-2.0](LICENSES/Apache-2.0.txt) |
| whisper.cpp / ggml 1.9.4 | 22, 41, 90 | MIT; [license](LICENSES/whisper.cpp/LICENSE) and retained notices |
| x265 3.4 | 15, 17, 23, 25, 52, 53, 54, 66, 71, 75, 84, 91, 92, 93, 94, 95, 96 | GPL-2.0-or-later; [COPYING](LICENSES/x265/COPYING). Upstream's separate commercial option is **not** granted here |
| XZ Utils 5.8.4 | 06, 50 | 0BSD; [text](LICENSES/xz/COPYING.0BSD) |
| zstd 1.5.7 | 18, 49, 99 | Upstream BSD-style **or** GPL-2.0; [BSD license](LICENSES/zstd/LICENSE) chosen for this distribution |

## Required credits and extraction changes

Portions of these artifacts are copyright © 2026 The FreeType Project
(https://freetype.org). All rights reserved. The FreeType-derived kernels are
26 and 27; see their retained notices and the bundled FreeType License.

This software is based in part on the work of the Independent JPEG Group.
The IJG/libjpeg-turbo-derived kernels are 16, 45, 55, and 56.

These kernels are **standalone extractions**, not copies of the complete
upstream projects. Only selected functions, declarations, tables, and macros
were retained; surrounding code and unused dependencies were omitted, and
some types, helpers, names, linkage, or calling interfaces were adapted for
isolation. Benchmark wrappers, differential tests, and RVV variants were added.
The source comments identify the original files and selected regions. These
statements document modifications for the FreeType and IJG source
redistribution terms; the upstream files themselves were not redistributed
unchanged.

## Redistribution

Distribute this file, `LICENSES/`, and the kernel source/header notices together.
Do not describe the entire artifact as CC0. The repository does not include
compiled kernel binaries; distributing compiled tests or other combinations
may entail additional obligations under the applicable upstream licenses.
Check that benchmark contributors have consented to CC0 for their own portions
and review licensing of any additional material before publishing an archive.
