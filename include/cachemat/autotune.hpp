#pragma once

#include "cachemat/matmul.hpp"
#include <iostream>

namespace cachemat {
// Simple autotuner
MatmulConfig autotune_matmul(int M, int N, int K, int trials = 3);
} // namespace cachemat