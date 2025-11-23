# cachemat

**cachemat** is a small C++ playground for exploring **cache-aware matrix multiplication**:

- Naive `O(M·N·K)` triple-loop matmul  
- Cache-blocked / tiled matmul  
- A simple **autotuner** that searches over block sizes and reports the fastest configuration  

The goal isn’t to beat BLAS, but to make it easy to *see* how tiling and cache behavior affect performance, and to have a concrete codebase you can link from blog posts or talks.

---

## Features

- **Naive vs blocked matmul** for `C = A × B` (row-major, single precision)
- Command-line benchmark driver: `bench_matmul`
- **Autotuning mode** that sweeps block sizes and reports the best configuration
- Prints:
  - Matrix dimensions (`M`, `N`, `K`)
  - Chosen mode (`naive`, `blocked`, `auto`)
  - Time in seconds
  - Derived GFLOP/s (assuming `2·M·N·K` flops)
  - A **checksum** for correctness sanity checks

---

## Project Layout

*(Adjust this if your folders differ slightly.)*

```text
cachemat/
├── include/
│   └── cachemat/         # Public headers (matmul APIs, utilities)
├── src/                  # Naive / blocked / autotune implementations
├── CMakeLists.txt
└── benchmarks/           # bench_matmul target (depending on CMake setup)
```

## Building
```bash
mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -ffast-math" \
      ..
cmake --build . -j
```
This should produce a bench_matmul binary under build/ (or build/benchmarks/, depending on your CMake config).

Flags:

`-O3` – aggressive optimization

`-march=native` – tune codegen for your local CPU (Apple Silicon / x86_64)

`-ffast-math`  – allow FP reassociation and FMA (great for benchmarking, not always for strict numerics)

## Usage
```bash
./bench_matmul \
  --m <M> \
  --n <N> \
  --k <K> \
  --mode <naive|blocked|auto> \
  [--block-m BM --block-n BN --block-k BK] \
  [--iters ITERS]
```
