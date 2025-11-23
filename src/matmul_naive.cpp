#include "cachemat/matmul.hpp"
#include <stdexcept>

namespace cachemat {
    Matrix matmul_naive(const Matrix& A, const Matrix& B) {
        if (A.cols() != B.rows()) 
            throw std::runtime_error("Incompatible matrix dimensions for multiplication");
        
        std::size_t M = A.rows();
        std::size_t N = B.cols();
        std::size_t K = A.cols();

        Matrix C(M, N);

        // Naive triple nested loop for matrix multiplication.
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                double sum = 0.0;
                for (std::size_t k = 0; k < K; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                C(i, j) = sum;
            }
        }
        return C;
    }
} // namespace cachemat