#include "s21_matrix.h"

s21::Matrix::Matrix(size_t rows, size_t cols) { setSizeMatrix(rows, cols); }

size_t s21::Matrix::getNumRows() const { return matrix_.size(); }

size_t s21::Matrix::getNumColumns() const {
  if (getNumRows() > 0) {
    return matrix_[0].size();
  }
  return 0;
}

double &s21::Matrix::operator()(int row, int col) { return matrix_[row][col]; }

double &s21::Matrix::operator()(int row, int col) const {
  return const_cast<double &>(matrix_[row][col]);
}

void s21::Matrix::setSizeMatrix(size_t rows, size_t cols) {
  matrix_.resize(rows);
  for (size_t i = 0; i < rows; ++i) {
    matrix_[i].resize(cols);
  }
}
