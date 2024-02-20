#include "s21_graph.h"

void s21::Graph::loadGraphFromFile(string &filename) {
  ifstream in(filename);
  if (!in.is_open()) {
    throw std::invalid_argument("Такого файла не существует");
  }
  in >> size_;
  matrix_ = s21::Matrix(size_, size_);
  for (size_t r = 0; r < size_; ++r) {
    for (size_t c = 0; c < size_; ++c) {
      in >> matrix_(r, c);
    }
  }
}

double &s21::Graph::operator()(size_t row, size_t col) const {
  return const_cast<double &>(matrix_(row, col));
}

size_t s21::Graph::getSize() const { return size_; }
