#!/usr/bin/env bash
set -euo pipefail

CXX=${CXX:-g++}
CXXFLAGS="-std=c++23 -I. -O2 -DLOCAL -Wall -lm"

run_one () {
  local src="$1"
  local out="/tmp/$(basename "$src" .cpp)"
  echo "[BUILD] $src"
  $CXX $CXXFLAGS "$src" -o "$out"
  echo "[RUN] $out"
  "$out"
}

run_one tests/unit/test_barrett.cpp
run_one tests/unit/test_lazy_seg_tree.cpp
run_one tests/unit/test_integer_set.cpp

echo "[FINISHED]"
