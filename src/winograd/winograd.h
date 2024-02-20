#ifndef A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_H_
#define A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_H_

#include <atomic>
#include <fstream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
using namespace std;
namespace s21 {

typedef vector<vector<int>> matrix;

class Winograd {
 public:
  void initMatrix(matrix &m, size_t rows, size_t cols);
  void fillMatrix(matrix &m);
  matrix multMatrix(const matrix &a, const matrix &b);
  void loadMatrixFromFile(const string &filename, matrix &a, matrix &b);
  matrix pipelineWinograd(const matrix &a, const matrix &b);
  matrix multWinograd(const matrix &a, const matrix &b, bool use_thread = false,
                      int count_threads = 0);
  void parallel_for(int count_threads, const matrix &a, const matrix &b, int d,
                    int b_cols, int count_iter, vector<int> rowFactor,
                    vector<int> colFactor, matrix &result);

 private:
  int random100();
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_H_
