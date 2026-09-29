#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../build.sh"

BUILD_DIR="${SCRIPT_DIR}/build"
INCLUDE_DIR="${SCRIPT_DIR}/include"
SRC_DIR="${SCRIPT_DIR}/src"
TEST_DIR="${SCRIPT_DIR}/tests"
VARIANTS_DIR="${SCRIPT_DIR}/variants"

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

KERNEL_FLAGS=(
  -I"${INCLUDE_DIR}"
  -fopenmp-simd
)

compile_kernel "${KERNEL_FLAGS[@]}" \
  -S "${SRC_DIR}/kernel.cpp" \
  -o "${BUILD_DIR}/assembly.s"

compile_kernel "${KERNEL_FLAGS[@]}" \
  "${SRC_DIR}/kernel.cpp" \
  "${TEST_DIR}/differential.cpp" \
  "${VARIANTS_DIR}/rvv.cpp" \
  -o "${BUILD_DIR}/test"
