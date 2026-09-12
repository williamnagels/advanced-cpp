# Ranges pipeline benchmark

This benchmark compares two implementations of the same operation:

1. Keep even values greater than or equal to `1000`.
2. Transform each accepted value with `value * 3 + 7`.
3. Sum the transformed values into a `std::uint64_t`.

`raw_pipeline` uses a loop and `ranges_pipeline` uses
`views::filter | views::transform`. Input construction and correctness checks
are outside the timed region.

Run every command in the training container from the repository root:

```sh
docker run --rm \
  --mount type=bind,source="$PWD",target=/workspace \
  cpptraining cmake -S /workspace/benchmarks/ranges \
    -B /workspace/build/ranges-benchmark -G Ninja \
    -DCMAKE_BUILD_TYPE=Release

docker run --rm \
  --mount type=bind,source="$PWD",target=/workspace \
  cpptraining cmake --build /workspace/build/ranges-benchmark

docker run --rm \
  --mount type=bind,source="$PWD",target=/workspace \
  cpptraining ctest --test-dir /workspace/build/ranges-benchmark \
    --output-on-failure

docker run --rm \
  --cpuset-cpus=0 \
  --mount type=bind,source="$PWD",target=/workspace \
  cpptraining /workspace/build/ranges-benchmark/ranges_pipeline_benchmark \
    --benchmark_enable_random_interleaving=true \
    --benchmark_out=/workspace/build/ranges-benchmark/results.json \
    --benchmark_out_format=json

docker run --rm \
  --mount type=bind,source="$PWD",target=/workspace \
  cpptraining sh /workspace/benchmarks/ranges/generate_assembly.sh
```

Assembly is generated for GCC and Clang with normal `-O3` optimization and
with loop vectorization explicitly disabled. Comparing each pair shows whether
vectorization changed either implementation; do not infer SIMD merely from
using `-O3`.

The benchmark covers all-pass, none-pass, alternating, and randomly mixed
inputs at 1 Ki, 64 Ki, and 4 Mi elements. Google Benchmark runs nine
repetitions of at least 0.2 seconds and reports aggregate statistics. Random
interleaving reduces ordering bias, while CPU pinning reduces migration noise.
Treat the numbers as properties of the tested compiler, CPU, and input
distribution rather than universal performance claims.