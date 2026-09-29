#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TEST_JOBS="${TEST_JOBS:-128}"
if [[ ! "$TEST_JOBS" =~ ^[1-9][0-9]*$ ]]; then
  printf 'TEST_JOBS must be a positive integer\n' >&2
  exit 2
fi

projects=("${SCRIPT_DIR}"/*/)
total=${#projects[@]}
completed=0
failed=0
active_pids=()
active_projects=()
temp_dir="$(mktemp -d)"
trap 'rm -rf "$temp_dir"' EXIT

finish_one() {
  local pid="${active_pids[0]}" project="${active_projects[0]}" name
  local log status
  active_pids=("${active_pids[@]:1}")
  active_projects=("${active_projects[@]:1}")

  name="${project%/}"
  name="${name##*/}"
  log="${project}build/test.log"
  if wait "$pid"; then
    status=OK
  else
    status=FAILED
    ((failed+=1))
  fi
  ((completed+=1))
  mkdir -p "${project}build"
  mv "${temp_dir}/${name}.log" "$log"
  if [[ "$status" == OK ]]; then
    printf '[%d/%d] OK     %s\n' "$completed" "$total" "$name"
  else
    printf '[%d/%d] FAILED %s (see %s)\n' "$completed" "$total" "$name" "$log" >&2
  fi
}

for project in "${projects[@]}"; do
  name="${project%/}"
  name="${name##*/}"
  bash "${project}test.sh" > "${temp_dir}/${name}.log" 2>&1 &
  active_pids+=("$!")
  active_projects+=("$project")
  if ((${#active_pids[@]} >= TEST_JOBS)); then
    finish_one
  fi
done

while ((${#active_pids[@]})); do
  finish_one
done

printf 'Test complete: %d/%d succeeded, %d failed\n' "$((completed - failed))" "$total" "$failed"
if ((failed)); then
  exit 1
fi
