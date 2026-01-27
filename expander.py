#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
expander.py

Written by ChatGPT (GPT-5.2 Thinking).

Behavior:
- Expand only quote-includes that resolve inside lib/... into one file.
- Ignore all '#pragma once' lines (remove them from output).
- Preserve original comments from source files; do NOT add any extra comments to generated C++ output.
- When an include expands a new file, insert a blank line after the expanded chunk.
- After all operations:
  - strip trailing spaces/tabs at end of each line
  - compress multiple consecutive blank lines to at most ONE blank line
"""

import argparse
import re
import sys
from pathlib import Path

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^">]+)([>"])\s*$')
PRAGMA_ONCE_RE = re.compile(r'^\s*#\s*pragma\s+once\s*(?://.*)?$')

def is_within(child: Path, parent: Path) -> bool:
    try:
        child.resolve().relative_to(parent.resolve())
        return True
    except Exception:
        return False

def read_text(p: Path) -> str:
    return p.read_text(encoding="utf-8", errors="replace")

def ensure_one_blank_line_after_chunk(out_lines: list[str]) -> None:
    tail = "".join(out_lines[-3:]) if out_lines else ""
    if tail.endswith("\n\n"):
        return
    if tail.endswith("\n"):
        out_lines.append("\n")
    else:
        out_lines.append("\n\n")

def expand_file(
    file_path: Path,
    *,
    project_root: Path,
    lib_dir: Path,
    included: set[Path],
    stack: list[Path],
) -> list[str]:
    file_path = file_path.resolve()

    if file_path in included:
        return []

    if file_path in stack:
        cycle = " -> ".join(str(p) for p in stack + [file_path])
        raise RuntimeError(f"Include cycle detected:\n{cycle}")

    if not file_path.exists():
        raise FileNotFoundError(f"File not found: {file_path}")

    included.add(file_path)
    stack.append(file_path)

    out_lines: list[str] = []
    base_dir = file_path.parent

    for line in read_text(file_path).splitlines(keepends=True):
        if PRAGMA_ONCE_RE.match(line):
            continue

        m = INCLUDE_RE.match(line)
        if not m:
            out_lines.append(line)
            continue

        bracket, inc = m.group(1), m.group(2).strip()

        if bracket == "<":
            out_lines.append(line)
            continue

        cand = (base_dir / inc).resolve()
        if not cand.exists():
            cand = (project_root / inc).resolve()

        if cand.exists() and is_within(cand, lib_dir):
            inlined = expand_file(
                cand,
                project_root=project_root,
                lib_dir=lib_dir,
                included=included,
                stack=stack,
            )
            if inlined:
                out_lines.extend(inlined)
                ensure_one_blank_line_after_chunk(out_lines)
        else:
            out_lines.append(line)

    stack.pop()
    return out_lines

def postprocess(text: str) -> str:
    # 1) strip trailing spaces/tabs per line
    lines = text.splitlines()
    lines = [ln.rstrip(" \t") for ln in lines]

    # 2) compress consecutive blank lines to at most ONE blank line
    out: list[str] = []
    blank_run = 0
    for ln in lines:
        if ln == "":
            blank_run += 1
            if blank_run <= 1:
                out.append("")
        else:
            blank_run = 0
            out.append(ln)

    # Keep a final newline (common in submissions)
    return "\n".join(out) + "\n"

def main():
    ap = argparse.ArgumentParser(
        description="Inline local includes under lib/... into a single C++ file (keep includes containing both 'lib' and 'debug'; ignore #pragma once; preserve original comments; add no extra comments; blank line between inlined files; strip trailing spaces; compress blank lines)."
    )
    ap.add_argument("entry", help="Entry .cpp (e.g. test.cpp).")
    ap.add_argument("-o", "--out", default="-", help="Output file (default: stdout). Use '-' for stdout.")
    ap.add_argument("--root", default=".", help="Project root (default: current dir).")
    ap.add_argument("--lib-dir", default="lib", help="Library directory to inline (default: lib).")
    args = ap.parse_args()

    project_root = Path(args.root).resolve()
    lib_dir = (project_root / args.lib_dir).resolve()
    entry = (project_root / args.entry).resolve()

    if not lib_dir.exists():
        print(f"[expander] ERROR: lib dir not found: {lib_dir}", file=sys.stderr)
        sys.exit(1)
    if not entry.exists():
        print(f"[expander] ERROR: entry not found: {entry}", file=sys.stderr)
        sys.exit(1)

    included: set[Path] = set()

    expanded = "".join(
        expand_file(
            entry,
            project_root=project_root,
            lib_dir=lib_dir,
            included=included,
            stack=[],
        )
    )

    expanded = postprocess(expanded)

    if args.out == "-":
        sys.stdout.write(expanded)
    else:
        out_path = Path(args.out).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(expanded, encoding="utf-8")

if __name__ == "__main__":
    main()
