#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>

namespace cachemat {
    class Matrix {
        public: 
            Matrix() = default; 

            Matrix(std::size_t rows, std::size_t cols)
                : m_rows(rows), m_cols(cols), m_data(rows * cols, 0.0) {}

            std::size_t rows() const { return m_rows; } // Get number of rows
            std::size_t cols() const { return m_cols; } // Get number of columns

            double * data() { return m_data.data(); } // Get pointer to data
            const double * data() const { return m_data.data(); } // Get const pointer to data

            double& operator()(std::size_t row, std::size_t col) { // Access element at (row, col)
                if (row >= m_rows || col >= m_cols) {
                    throw std::out_of_range("Matrix indices out of range");
                }
                return m_data[row * m_cols + col];
            }
            const double& operator()(std::size_t row, std::size_t col) const { // Access element at (row, col) const version
                if (row >= m_rows || col >= m_cols) {
                    throw std::out_of_range("Matrix indices out of range");
                }
                return m_data[row * m_cols + col];
            }
            void fill(double value); // Fill matrix with a specific value
    private:
        std::size_t m_rows = 0;
        std::size_t m_cols = 0;
        std::vector<double> m_data;
    };
} // namespace cachemat