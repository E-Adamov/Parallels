#ifndef A3_PARALLELS_SRC_ANT_INTERFACE_H_
#define A3_PARALLELS_SRC_ANT_INTERFACE_H_

#include <cstdlib>
#include <iostream>

#include "../support/s21_graph.h"
#include "../support/s21_screen.h"
#include "../support/s21_simple_timer.h"
#include "ant_colony.h"
using namespace std;

namespace s21 {
class AntInterface {
 public:
  AntInterface() = default;
  void start();

 private:
  s21::Graph graph_;
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_ANT_INTERFACE_H_
