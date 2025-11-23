#include "cachemat/autotune.hpp"
#include "cachemat/matrix.hpp"
#include <chrono>
#include <limits>
#include <random>
#include <vector>

namespace {
double benchmark_cfg(int M, int N, int K, const cachemat::MatmulConfig &config, int trials) {
  // Create random matrices A and B
  cachemat::Matrix A(M, K);
  cachemat::Matrix B(K, N);

  std::mt19937                     gen(42); // Fixed seed for reproducibility
  std::uniform_real_distribution<> dis(-1.0, 1.0);

  // Fill matrices with random data
  for (std::size_t i = 0; i < M; ++i)
    for (std::size_t k = 0; k < K; ++k)
      A(i, k) = dis(gen);
  for (std::size_t k = 0; k < K; ++k)
    for (std::size_t j = 0; j < N; ++j)
      B(k, j) = dis(gen);

  // Warmup
  (void) cachemat::matmul_blocked(A, B, config);

  // Benchmarking
  using clock      = std::chrono::high_resolution_clock;
  double best_time = std::numeric_limits<double>::max();

  for (int timestep = 0; timestep < trials; ++timestep) {
    auto start = clock::now();
    (void) cachemat::matmul_blocked(A, B, config);
    auto                          end     = clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() < best_time) {
      best_time = elapsed.count();
    }
  }
  return best_time;
}
} // anonymous namespace

namespace cachemat {
MatmulConfig autotune_matmul(int M, int N, int K, int trials) {
  std::vector<int> block_sizes = {16, 32, 64, 128};

  MatmulConfig best_config;
  double       best_time = std::numeric_limits<double>::max();

  for (int bm : block_sizes) {
    for (int bn : block_sizes) {
      for (int bk : block_sizes) {
        std::cout << "Testing config: blockM=" << bm << ", blockN=" << bn << ", blockK=" << bk
                  << "\n";
        MatmulConfig config;
        config.blockM = bm;
        config.blockN = bn;
        config.blockK = bk;

        double time = benchmark_cfg(M, N, K, config, trials);
        if (time < best_time) {
          best_time   = time;
          best_config = config;
        }
      }
    }
  }
  return best_config;
}
} // namespace cachemat