#ifndef A3_PARALLELS_SRC_SUPPORT_S21_GRAPH_H_
#define A3_PARALLELS_SRC_SUPPORT_S21_GRAPH_H_
// https://programforyou.ru/graph-redactor

#include <fstream>

#include "s21_matrix.h"
using namespace std;

namespace s21 {

class Graph {
 public:
  Graph() = default;
  void loadGraphFromFile(string &filename);
  void exportGraphToDot(string filename);
  size_t getSize() const;
  bool empty();
  double &operator()(size_t row, size_t col);
  double &operator()(size_t row, size_t col) const;

 private:
  s21::Matrix matrix_;
  size_t size_{};
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_SUPPORT_S21_GRAPH_H_
