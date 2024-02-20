#ifndef A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_INTERFACE_H_
#define A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_INTERFACE_H_

#include <iomanip>  // std::setprecision(10)
#include <iostream>

#include "../support/s21_screen.h"
#include "../support/s21_simple_timer.h"
#include "winograd.h"
using namespace std;

namespace s21 {
class WinogradInterface {
 public:
  WinogradInterface() = default;

  void start();

 private:
  s21::Winograd w_;
  size_t count_iter{1}, a_rows{1}, a_cols{1}, b_rows{1}, b_cols{1};
  bool print_matrix{false}, mult{true}, mult_vinograd{true}, classic{true},
      pipeline{true};

  void printResult(matrix &a, matrix &b);
  void printMatrix(vector<vector<int>> &m);
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_WINOGRAD_WINOGRAD_INTERFACE_H_
