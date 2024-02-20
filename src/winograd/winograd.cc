#include "winograd.h"

int s21::Winograd::random100() {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(-9, 9);
  return (static_cast<int>(dis(gen)));
}

void s21::Winograd::initMatrix(matrix &m, size_t rows, size_t cols) {
  if (rows <= 0 || cols <= 0)
    throw std::invalid_argument("Размер матрицы должен быть больше нуля");
  m.resize(rows);
  for (size_t i = 0; i < rows; ++i) {
    m[i].resize(cols);
  }
}

void s21::Winograd::fillMatrix(matrix &m) {
  for (size_t r = 0; r < m.size(); ++r) {
    for (size_t c = 0; c < m[r].size(); ++c) {
      m[r][c] = random100();
    }
  }
}

s21::matrix s21::Winograd::multMatrix(const matrix &a, const matrix &b) {
  matrix c;
  initMatrix(c, a.size(), b[0].size());
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < b[0].size(); ++j) {
      for (size_t k = 0; k < b.size(); ++k) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return c;
}

void s21::Winograd::loadMatrixFromFile(const string &filename, matrix &a,
                                       matrix &b) {
  ifstream in(filename);
  if (!in.is_open()) {
    throw std::invalid_argument("Такого файла не существует");
  }
  size_t a_rows{}, a_cols{}, b_rows{}, b_cols{};
  in >> a_rows;
  in >> a_cols;
  initMatrix(a, a_rows, a_cols);
  for (size_t r = 0; r < a_rows; ++r) {
    for (size_t c = 0; c < a_cols; ++c) {
      in >> a[r][c];
    }
  }
  in >> b_rows;
  in >> b_cols;
  initMatrix(b, b_rows, b_cols);
  for (size_t r = 0; r < b_rows; ++r) {
    for (size_t c = 0; c < b_cols; ++c) {
      in >> b[r][c];
    }
  }
}

s21::matrix s21::Winograd::pipelineWinograd(const matrix &a, const matrix &b) {
  size_t a_rows = a.size();
  size_t a_cols = a[0].size();
  size_t b_rows = b.size();
  size_t b_cols = b[0].size();
  size_t d = a_cols / 2;
  if (a_cols != b_rows)
    throw std::invalid_argument(
        "Матрицы не могут быть умножены: кол-во "
        "столбцов не равно кол-ву строк ");
  matrix result;
  initMatrix(result, a_rows, b_cols);
  vector<int> rowFactor(a_rows);
  vector<int> colFactor(b_cols);
  bool is_even_columns = a_cols % 2 == 0 ? true : false;

  vector<int> a_vec;
  vector<int> b_vec;
  a_vec.reserve(a_rows * a_cols);
  b_vec.reserve(b_rows * b_cols);
  atomic<size_t> row_num{};

  auto stage_1 = [&]() {
    for (size_t i = 0; i < a_rows; ++i) {
      for (size_t k = 0; k < d; ++k) {
        int r1 = a[i][2 * k];
        int r3 = a[i][2 * k + 1];
        rowFactor[i] += r1 * r3;
        a_vec.push_back(r1);
        a_vec.push_back(r3);
      }
      if (!is_even_columns) a_vec.push_back(0);
    }
  };

  auto stage_2 = [&]() {
    for (size_t j = 0; j < b_cols; ++j) {
      for (size_t k = 0; k < d; ++k) {
        int r4 = b[2 * k][j];
        int r2 = b[2 * k + 1][j];
        colFactor[j] += r4 * r2;
        b_vec.push_back(r4);
        b_vec.push_back(r2);
      }
      if (!is_even_columns) b_vec.push_back(0);
    }
  };
  auto stage_3 = [&]() {
    for (size_t i = 0; i < a_rows; ++i) {
      row_num = i;
      for (size_t j = 0; j < b_cols; ++j) {
        while (true) {
          if (a_vec.size() >= (i + 1) * a_cols &&
              b_vec.size() >= (j + 1) * b_rows) {
            result[i][j] = -rowFactor[i] - colFactor[j];
            for (size_t k = 0; k < d; ++k) {
              result[i][j] +=
                  (a_vec[i * a_cols + 2 * k] + b_vec[2 * k + 1 + j * b_rows]) *
                  (a_vec[i * a_cols + 2 * k + 1] + b_vec[2 * k + j * b_rows]);
            }
            break;
          }
        }
      }
    }
    ++row_num;
  };

  auto stage_4 = [&]() {
    if (!is_even_columns) {
      for (size_t i = 0; i < a_rows; ++i) {
        while (true) {
          if (row_num - 1 >= i) {
            for (size_t j = 0; j < b_cols; ++j) {
              result[i][j] += a[i][a_cols - 1] * b[a_cols - 1][j];
            }
            break;
          }
        }
      }
    }
  };

  thread th1(stage_1);
  thread th2(stage_2);
  thread th3(stage_3);
  thread th4(stage_4);
  th1.join();
  th2.join();
  th3.join();
  th4.join();

  return result;
}

s21::matrix s21::Winograd::multWinograd(const matrix &a, const matrix &b,
                                        bool use_thread, int count_threads) {
  int a_rows = a.size();
  int a_cols = a[0].size();
  int b_rows = b.size();
  int b_cols = b[0].size();
  int d = a_cols / 2;
  if (a_cols != b_rows)
    throw std::invalid_argument(
        "Матрицы не могут быть умножены: кол-во "
        "столбцов не равно кол-ву строк ");
  matrix result;
  initMatrix(result, a_rows, b_cols);
  vector<int> rowFactor(a_rows);
  vector<int> colFactor(b_cols);
  bool is_even_columns = a_cols % 2 == 0 ? true : false;

  for (int i = 0; i < a_rows; ++i) {
    for (int j = 0; j < d; ++j) {
      rowFactor[i] += a[i][2 * j] * a[i][2 * j + 1];
    }
  }
  for (int i = 0; i < b_cols; ++i) {
    for (int j = 0; j < d; ++j) {
      colFactor[i] += b[2 * j][i] * b[2 * j + 1][i];
    }
  }
  if (use_thread) {
    parallel_for(count_threads, a, b, d, b_cols, a_rows, rowFactor, colFactor,
                 result);
  } else {
    for (int i = 0; i < a_rows; ++i) {
      for (int j = 0; j < b_cols; ++j) {
        result[i][j] = -rowFactor[i] - colFactor[j];
        for (int k = 0; k < d; ++k) {
          result[i][j] +=
              (a[i][2 * k] + b[2 * k + 1][j]) * (a[i][2 * k + 1] + b[2 * k][j]);
        }
      }
    }
  }

  if (!is_even_columns) {
    for (int i = 0; i < a_rows; ++i) {
      for (int j = 0; j < b_cols; ++j) {
        result[i][j] += a[i][a_cols - 1] * b[a_cols - 1][j];
      }
    }
  }

  return result;
}

void s21::Winograd::parallel_for(int count_threads, const matrix &a,
                                 const matrix &b, int d, int b_cols,
                                 int count_iter, vector<int> rowFactor,
                                 vector<int> colFactor, matrix &result) {
  int max_threads = std::thread::hardware_concurrency();
  max_threads = max_threads == 0 ? 2 : max_threads;
  int input_threads = count_threads;
  input_threads = input_threads == 0 ? max_threads : input_threads;
  int num_threads = count_iter < input_threads ? count_iter : input_threads;
  int shift_thread = count_iter / num_threads;
  std::vector<std::thread> threads(num_threads);

  auto one_thread = [&](int begin, int end) {
    for (auto i = begin; i != end; ++i) {
      for (int j = 0; j < b_cols; ++j) {
        result[i][j] = -rowFactor[i] - colFactor[j];
        for (int k = 0; k < d; ++k) {
          result[i][j] +=
              (a[i][2 * k] + b[2 * k + 1][j]) * (a[i][2 * k + 1] + b[2 * k][j]);
        }
      }
    }
  };

  auto s_begin = 0;
  auto s_end = count_iter;

  for (size_t i = 0; i < threads.size() - 1; ++i) {
    threads[i] = std::thread(one_thread, s_begin, s_begin + shift_thread);
    s_begin = s_begin + shift_thread;
  }
  threads[threads.size() - 1] = std::thread(one_thread, s_begin, s_end);

  for (auto &thr : threads) {
    thr.join();
  }
}
