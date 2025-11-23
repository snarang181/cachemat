#pragma once

#include "cachemat/matrix.hpp"

namespace cachemat {
    struct MatmulConfig {
        int blockM = 32; // Block size for M dimension
        int blockN = 32; // Block size for N dimension
        int blockK = 32; // Block size for K dimension
    }; // struct MatmulConfig

    Matrix matmul_naive(const Matrix& A, const Matrix& B); // Naive matrix multiplication
    
    // C = A*B using blocked/tiled approach
    Matrix matmul_blocked(const Matrix& A, const Matrix& B, const MatmulConfig& config);
} // namespace cachemat