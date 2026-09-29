#!/bin/bash
set -euo pipefail

BUILD_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

RUNNER=(qemu-riscv64 -L /opt/riscv-gnu-toolchain-2026-08-27/sysroot -cpu rv64,v=true,vlen=128,vext_spec=v1.0)

run_test_binary() {
    local kernel output status
    kernel="$(basename "$(dirname "$(dirname "$1")")")"

    if output="$("${RUNNER[@]}" "$@" 2>&1)"; then
        printf 'OK     %s\n' "$kernel"
    else
        status=$?
        printf 'FAILED %s (exit %d)\n' "$kernel" "$status" >&2
        if [[ -n "$output" ]]; then
            printf '%s\n' "$output" >&2
        fi
        return "$status"
    fi
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
    exec bash "${BUILD_ROOT}/kernels/test.sh" "$@"
fi
