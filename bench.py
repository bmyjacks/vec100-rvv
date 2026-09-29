#!/usr/bin/env python3
"""Analyze the assembly and vectorization reports produced by build.sh.

Run `bash build.sh` first. This script reads the existing build outputs without
recompiling any kernels.
"""

import argparse
import csv
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


ROOT = Path(__file__).resolve().parent
FIELDS = ("kernel_id", "compiler", "compiler_version", "status", "loop_sites",
          "slp_sites", "rvv_instructions", "error")
VECTOR_INSTRUCTION = re.compile(r"^\s+(v[a-z][a-z0-9_.]*)\s", re.M)
GCC_LOOP = re.compile(r"optimized:.*\bloops? vectorized\b", re.I)
GCC_SLP = re.compile(r"optimized:.*\b(?:basic block part|SLP) vectorized\b", re.I)


def compiler_config():
    # Source only the root configuration, never the package scripts (which
    # remove build/). No kernel is compiled by this script.
    proc = subprocess.run(
        ["bash", "-c", 'source "$1"; printf "%s" "$CXX"',
         "bash", str(ROOT / "build.sh")],
        cwd=ROOT, capture_output=True, check=True,
    )
    compiler = proc.stdout.decode()
    if not compiler:
        raise ValueError("build.sh did not define CXX")
    version = subprocess.run([compiler, "--version"], cwd=ROOT, text=True,
                             capture_output=True, check=True).stdout.splitlines()[0]
    if "clang" in version.lower():
        family = "clang"
    elif ("gcc" in version.lower() or "g++" in version.lower()
          or "g++" in Path(compiler).name or "gcc" in Path(compiler).name):
        family = "gcc"
    else:
        raise ValueError(f"Unsupported compiler: {version}")
    return compiler, family, version


def rvv_instructions(path):
    text = path.read_text(errors="replace")
    return [match.group(1) for match in VECTOR_INSTRUCTION.finditer(text)
            if not match.group(1).startswith("vset")]


def clang_sites(record):
    text = record.read_text(errors="replace")
    loop = slp = 0
    for block in re.split(r"(?=^--- !)", text, flags=re.M):
        if not block.startswith("--- !Passed"):
            continue
        if re.search(r"^Pass:\s+loop-vectorize\s*$", block, re.M) and re.search(
                r"^Name:\s+Vectorized\s*$", block, re.M):
            loop += 1
        if re.search(r"^Pass:\s+slp-vectorizer\s*$", block, re.M):
            slp += 1
    return loop, slp


def measure(ident, family):
    package = ROOT / "kernels" / ident
    source = package / "src/kernel.cpp"
    built = package / "build/assembly.s"
    report = package / "build/vectorization.txt"
    record = package / "build/vectorization.yaml"
    inputs = (built, report, record) if family == "clang" else (built, report)
    for path in inputs:
        if not path.is_file():
            raise ValueError(f"missing {path}; run bash build.sh first")
    latest_source = max(path.stat().st_mtime_ns for path in
                        (ROOT / "build.sh", package / "build.sh", source))
    if any(path.stat().st_mtime_ns < latest_source for path in inputs):
        raise ValueError(f"{package / 'build'} predates build.sh or its source; rebuild first")
    if family == "clang":
        loops, slp = clang_sites(record)
    else:
        text = report.read_text(errors="replace")
        loops, slp = len(GCC_LOOP.findall(text)), len(GCC_SLP.findall(text))
    built_rvv = rvv_instructions(built)
    status = "loop+slp" if loops and slp and built_rvv else (
        "loop" if loops and built_rvv else "slp" if slp and built_rvv else "no")
    return dict(kernel_id=ident, status=status, loop_sites=loops, slp_sites=slp,
                rvv_instructions=len(built_rvv), error="")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kernel", action="append", help="limit to a kernel ID (repeatable)")
    parser.add_argument("--output", type=Path, default=ROOT / "bench-results.csv")
    parser.add_argument("--jobs", type=int, default=128, help="parallel log readers (default: 128)")
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error("--jobs must be a positive integer")
    kernels_dir = ROOT / "kernels"
    ids = sorted(path.name for path in kernels_dir.iterdir() if path.is_dir())
    if not ids:
        parser.error(f"no kernel directories found in {kernels_dir}")
    if args.kernel:
        unknown = set(args.kernel) - set(ids)
        if unknown:
            parser.error(f"unknown kernel ID(s): {', '.join(sorted(unknown))}")
        ids = [ident for ident in ids if ident in args.kernel]
    try:
        compiler, family, version = compiler_config()
    except (OSError, subprocess.CalledProcessError, ValueError) as exc:
        parser.exit(1, f"Cannot read compiler from build.sh: {exc}\n")
    print(f"Compiler: {family.upper()} — {version}", flush=True)
    rows = []
    with ThreadPoolExecutor(max_workers=min(args.jobs, len(ids))) as pool:
        futures = [pool.submit(measure, ident, family) for ident in ids]
        for index, (ident, future) in enumerate(zip(ids, futures), 1):
            try:
                row = future.result()
            except (OSError, ValueError) as exc:
                row = dict(kernel_id=ident, status="error", loop_sites="", slp_sites="",
                           rvv_instructions="", error=str(exc))
            row.update(compiler=compiler, compiler_version=version)
            rows.append(row)
            print(f"[{index}/{len(ids)}] {row['status'].upper():<8} {ident}"
                  + (f" ({row['error']})" if row["error"] else ""),
                  file=sys.stderr if row["error"] else sys.stdout, flush=True)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=FIELDS)
        writer.writeheader()
        writer.writerows(rows)
    good = [row for row in rows if row["status"] != "error"]
    count = lambda predicate: sum(predicate(row) for row in good)
    print(f"Vectorized: {count(lambda r: r['status'] != 'no')}/{len(good)}; "
          f"loop: {count(lambda r: r['status'] in ('loop', 'loop+slp'))}; "
          f"SLP-only: {count(lambda r: r['status'] == 'slp')}; "
          f"both: {count(lambda r: r['status'] == 'loop+slp')}; "
          f"errors: {len(rows) - len(good)}\nCSV: {args.output}")
    return int(len(rows) != len(good))


if __name__ == "__main__":
    sys.exit(main())
