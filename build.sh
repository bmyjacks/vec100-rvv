#!/bin/bash
set -euo pipefail

BUILD_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

CXX="/opt/LLVM-23.1.1-Linux-X64/bin/clang++"
EXTRA_FLAGS=(
    --target=riscv64-unknown-linux-gnu
    --gcc-toolchain=/opt/riscv-gnu-toolchain-2026-08-27
    --sysroot=/opt/riscv-gnu-toolchain-2026-08-27/sysroot

    -march=rv64gcv
    -mabi=lp64d
    -O3
    -std=c++17
)

# CXX="/opt/riscv-gnu-toolchain-2026-08-27/bin/riscv64-unknown-linux-gnu-g++"
# EXTRA_FLAGS=(
#    --sysroot=/opt/riscv-gnu-toolchain-2026-08-27/sysroot
#
#    -march=rv64gcv
#    -mabi=lp64d
#    -O3
#    -std=c++17
# )

compile_kernel() {
    local assembly="" output_dir is_assembly=0
    local -a args=("$@") report_flags=()
    local i

    for ((i = 0; i < ${#args[@]}; i++)); do
        case "${args[i]}" in
            -S) is_assembly=1 ;;
            -o) assembly="${args[i+1]:-}" ;;
        esac
    done

    if [[ "$is_assembly" == 0 || -z "$assembly" ]]; then
        "$CXX" "${EXTRA_FLAGS[@]}" "${args[@]}"
        return
    fi

    case "$assembly" in
        */*) output_dir="${assembly%/*}" ;;
        *) output_dir=. ;;
    esac

    case "${CXX##*/}:$("$CXX" --version)" in
        *clang*)
            report_flags=(
                '-Rpass=loop-vectorize|slp-vectorizer'
                '-Rpass-missed=loop-vectorize|slp-vectorizer'
                '-Rpass-analysis=loop-vectorize|slp-vectorizer'
                -fsave-optimization-record
                "-foptimization-record-file=${output_dir}/vectorization.yaml"
            )
            ;;
        *GCC*|*gcc*|*g++*) report_flags=(-fopt-info-vec-all) ;;
        *)
            printf 'Unsupported compiler for vectorization reports: %s\n' "$CXX" >&2
            return 1
            ;;
    esac

    if "$CXX" "${EXTRA_FLAGS[@]}" "${report_flags[@]}" "${args[@]}" \
        > "${output_dir}/vectorization.txt" 2>&1; then
        return 0
    else
        local status=$?
        printf 'Assembly compilation failed; see %s/vectorization.txt\n' "$output_dir" >&2
        return "$status"
    fi
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
    exec bash "${BUILD_ROOT}/kernels/build.sh"
fi
