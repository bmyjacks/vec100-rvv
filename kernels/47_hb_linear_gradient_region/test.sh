#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../test.sh"

BUILD_DIR="${SCRIPT_DIR}/build"

run_test_binary "${BUILD_DIR}/test"
