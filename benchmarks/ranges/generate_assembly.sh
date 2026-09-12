#!/bin/sh
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repository_root=$(CDPATH= cd -- "$script_dir/../.." && pwd)
output_dir=${1:-"$repository_root/build/ranges-benchmark/assembly"}

mkdir -p "$output_dir"

g++ -std=c++23 -O3 -march=x86-64-v3 -masm=intel -S \
    "$script_dir/pipeline.cpp" -o "$output_dir/gcc-optimized.s"
g++ -std=c++23 -O3 -march=x86-64-v3 -fno-tree-vectorize -masm=intel -S \
    "$script_dir/pipeline.cpp" -o "$output_dir/gcc-no-vectorization.s"

clang++ -std=c++23 -O3 -march=x86-64-v3 -masm=intel -S \
    "$script_dir/pipeline.cpp" -o "$output_dir/clang-optimized.s"
clang++ -std=c++23 -O3 -march=x86-64-v3 -fno-vectorize -fno-slp-vectorize \
    -masm=intel -S "$script_dir/pipeline.cpp" \
    -o "$output_dir/clang-no-vectorization.s"

printf 'Assembly written to %s\n' "$output_dir"