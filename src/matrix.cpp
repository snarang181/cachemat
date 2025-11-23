#include "cachemat/matrix.hpp"

namespace cachemat {
    void Matrix::fill(double value) {
        std::fill(m_data.begin(), m_data.end(), value);
    }  
} // namespace cachemat