#include "cachemat/matmul.hpp"
#include <algorithm>
#include <stdexcept>

namespace cachemat {
Matrix matmul_blocked(const Matrix &A, const Matrix &B, const MatmulConfig &config) {
  if (A.cols() != B.rows())
    throw std::runtime_error("Incompatible matrix dimensions for multiplication");

  std::size_t M = A.rows();
  std::size_t N = B.cols();
  std::size_t K = A.cols();

  const int BM = config.blockM;
  const int BN = config.blockN;
  const int BK = config.blockK;

  Matrix C(M, N);

  // Blocked matrix multiplication
  for (std::size_t i0 = 0; i0 < M; i0 += BM) {            // Loop over blocks in M dimension
    std::size_t iMax = std::min<std::size_t>(i0 + BM, M); // Handle edge case

    for (std::size_t k0 = 0; k0 < K; k0 += BK) {            // Loop over blocks in K dimension
      std::size_t kMax = std::min<std::size_t>(k0 + BK, K); // Handle edge case

      for (std::size_t j0 = 0; j0 < N; j0 += BN) {            // Loop over blocks in N dimension
        std::size_t jMax = std::min<std::size_t>(j0 + BN, N); // Handle edge case

        // Multiply tile A(i0:iMax, k0:kMax) with tile B(k0:kMax, j0:jMax) and accumulate into
        // C(i0:iMax, j0:jMax)
        for (std::size_t i = i0; i < iMax; ++i) {
          for (std::size_t k = k0; k < kMax; ++k) {
            double a_ik = A(i, k);
            // Unroll innermost loop over j
            for (std::size_t j = j0; j < jMax; ++j) {
              C(i, j) += a_ik * B(k, j);
            }
          }
        }
      } // j0 loop
    }   // k0 loop
  }     // i0 loop
  return C;
}
} // namespace cachemat