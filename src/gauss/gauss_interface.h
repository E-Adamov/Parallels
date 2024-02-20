#ifndef A3_PARALLELS_SRC_GAUSS_GAUSS_INTERFACE_H_
#define A3_PARALLELS_SRC_GAUSS_GAUSS_INTERFACE_H_

#include <iomanip>  // std::setprecision(10)
#include <iostream>

#include "../support/s21_screen.h"
#include "../support/s21_simple_timer.h"
#include "gauss.h"
using namespace std;

namespace s21 {
class GaussInterface {
 public:
  GaussInterface() = default;
  void start();

 private:
  s21::Gauss gauss_;

  void printResult(const std::vector<double> &v);
  void printTime(float time);
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_GAUSS_GAUSS_INTERFACE_H_
