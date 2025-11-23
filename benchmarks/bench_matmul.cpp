#include "cachemat/autotune.hpp"
#include "cachemat/matmul.hpp"
#include "cachemat/matrix.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

using namespace cachemat;

struct Args {
  std::size_t M    = 1024;
  std::size_t N    = 1024;
  std::size_t K    = 1024;
  std::string mode = "naive"; // "naive", "blocked", "autotune"
};

Args parse_args(int argc, char **argv) {
  Args args;
  for (int i = 1; i < argc; ++i) {
    std::string arg     = argv[i];
    auto        get_val = [&](int &idx) {
      if (idx + 1 >= argc) {
        throw std::runtime_error("Missing value for argument: " + arg);
      }
      return argv[++idx]; // Return next argument as value
    };

    if (arg == "--m") {
      args.M = std::stoul(get_val(i)); // Convert to size_t
    } else if (arg == "--n") {
      args.N = std::stoul(get_val(i)); // Convert to size_t
    } else if (arg == "--k") {
      args.K = std::stoul(get_val(i)); // Convert to size_t
    } else if (arg == "--mode") {
      args.mode = get_val(i);
    } else {
      throw std::runtime_error("Unknown argument: " + arg);
    }
  }
  return args;
}

int main(int argc, char **argv) {
  try {
    Args args = parse_args(argc, argv);

    std::cout << "Matrix dimensions: M=" << args.M << ", N=" << args.N << ", K=" << args.K << "\n";
    std::cout << "Mode: " << args.mode << "\n";

    // Create random matrices A and B
    Matrix A(args.M, args.K);
    Matrix B(args.K, args.N);

    std::mt19937                     gen(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Fill matrices with random data
    for (std::size_t i = 0; i < args.M; ++i)
      for (std::size_t k = 0; k < args.K; ++k)
        A(i, k) = dis(gen);
    for (std::size_t k = 0; k < args.K; ++k)
      for (std::size_t j = 0; j < args.N; ++j)
        B(k, j) = dis(gen);

    Matrix C;

    if (args.mode == "naive") {
      auto start                            = std::chrono::high_resolution_clock::now();
      C                                     = matmul_naive(A, B);
      auto                          end     = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      std::cout << "Naive multiplication time: " << elapsed.count() << " seconds\n";
    } else if (args.mode == "blocked") {
      MatmulConfig config; // Default config
      auto         start                    = std::chrono::high_resolution_clock::now();
      C                                     = matmul_blocked(A, B, config);
      auto                          end     = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      std::cout << "Blocked multiplication time: " << elapsed.count() << " seconds\n";
    } else if (args.mode == "autotune") {
      MatmulConfig best_config = autotune_matmul(args.M, args.N, args.K, 3);
      std::cout << "Autotuned config: blockM=" << best_config.blockM
                << ", blockN=" << best_config.blockN << ", blockK=" << best_config.blockK << "\n";

      auto start                            = std::chrono::high_resolution_clock::now();
      C                                     = matmul_blocked(A, B, best_config);
      auto                          end     = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      std::cout << "Autotuned blocked multiplication time: " << elapsed.count() << " seconds\n";
    } else {
      throw std::runtime_error("Unknown mode: " + args.mode);
    }

    // Optionally verify result or print a checksum
    // Also, we will not allow compiler optimizations
    // to discard the computation C
    double checksum = 0.0;
    for (std::size_t i = 0; i < C.rows(); ++i)
      for (std::size_t j = 0; j < C.cols(); ++j)
        checksum += C(i, j);
    std::cout << "Result checksum: " << checksum << "\n";
    return 0;

  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
}