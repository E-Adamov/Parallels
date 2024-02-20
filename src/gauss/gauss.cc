#include "gauss.h"

std::vector<double> s21::Gauss::gauss(bool use_threads) {
  vector<double> res(size_);
  vector<double> v = vector_;
  vector<vector<double>> m = matrix_;
  // Прямой ход
  for (int k = 0; k < size_; ++k) {
    upTopMaxElement(k, m, v);
    if (use_threads) {
      parallel_for(k, m, v);
    } else {
      for (int i = k + 1; i < size_; ++i) {
        double temp = m[i][k] / m[k][k];
        for (int j = 0; j < size_; ++j) {
          m[i][j] = m[i][j] - m[k][j] * temp;
        }
        v[i] = v[i] - v[k] * temp;
      }
    }
  }
  // Обратный ход
  for (int k = size_ - 1; k >= 0; --k) {
    for (int i = k + 1; i < size_; ++i) v[k] = v[k] - m[k][i] * res[i];
    res[k] = v[k] / m[k][k];
  }

  return res;
}

vector<double> s21::Gauss::gauss_paralell(int count_iter) {
  int input_threads = std::thread::hardware_concurrency();
  input_threads = input_threads == 0 ? 2 : input_threads;
  int num_threads = count_iter < input_threads ? count_iter : input_threads;
  int shift_thread = count_iter / num_threads;
  std::vector<std::thread> threads(num_threads);

  vector<double> res(size_);
  auto one_thread = [&](int begin, int end) {
    for (auto j = begin; j != end; ++j) {
      gauss();
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
  return res = gauss();
}

void s21::Gauss::loadGaussFromFile(string &filename) {
  ifstream in(filename);
  if (!in.is_open()) {
    throw std::invalid_argument("Такого файла не существует");
  }
  in >> size_;
  resize(size_);
  for (int r = 0; r < size_; ++r) {
    for (int c = 0; c < size_ + 1; ++c) {
      if (c == size_) {
        in >> vector_[r];
      } else {
        in >> matrix_[r][c];
      }
    }
  }
}

void s21::Gauss::loadRandmGauss(int size) {
  if (size > 0) {
    size_ = size;
    resize(size);
    for (int r = 0; r < size; ++r) {
      for (int c = 0; c < size; ++c) {
        matrix_[r][c] = random100();
      }
    }
    for (unsigned i = 0; i < vector_.size(); ++i) {
      vector_[i] = random100();
    }
  } else {
    throw std::invalid_argument("Введите корректное число");
  }
}

int s21::Gauss::size() { return size_; }

bool s21::Gauss::empty() { return size_ == 0 ? true : false; }

void s21::Gauss::resize(int size) {
  vector_.resize(size);
  matrix_.resize(size);
  for (int i = 0; i < size; ++i) {
    matrix_[i].resize(size);
  }
}

void s21::Gauss::parallel_for(int k, vector<vector<double>> &m,
                              vector<double> &v) {
  int input_threads = std::thread::hardware_concurrency();
  int count_iter = m.size() - k;
  input_threads = input_threads == 0 ? 2 : input_threads;
  int num_threads = count_iter < input_threads ? count_iter : input_threads;
  int shift_thread = count_iter / num_threads;
  std::vector<std::thread> threads(num_threads);

  int n = m.size();
  auto one_thread = [&](int begin, int end) {
    for (int i = begin; i < end; i++) {
      double temp = m[i][k] / m[k][k];
      for (int j = 0; j < n; ++j) {
        m[i][j] = m[i][j] - m[k][j] * temp;
      }
      v[i] = v[i] - v[k] * temp;
    }
  };
  auto s_begin = k + 1;
  auto s_end = count_iter + k;
  for (size_t i = 0; i < threads.size() - 1; ++i) {
    threads[i] = std::thread(one_thread, s_begin, s_begin + shift_thread);
    s_begin = s_begin + shift_thread;
  }
  threads[threads.size() - 1] = std::thread(one_thread, s_begin, s_end);
  for (auto &thr : threads) {
    thr.join();
  }
}

void s21::Gauss::upTopMaxElement(int k, vector<vector<double>> &m,
                                 vector<double> &v) {
  int n = m.size();
  double max = abs(m[k][k]);
  int index = k;
  // Поиск строки с максимальным m[i][k]
  for (int i = k + 1; i < n; ++i) {
    if (abs(m[i][k]) > max) {
      max = abs(m[i][k]);
      index = i;
    }
  }
  // Перестановка строк
  if (max == 0) {
    throw runtime_error("Решение получить невозможно из-за нулевого столбца");
  }
  for (int j = 0; j < n; ++j) {
    double temp = m[k][j];
    m[k][j] = m[index][j];
    m[index][j] = temp;
  }
  double temp = v[k];
  v[k] = v[index];
  v[index] = temp;
}

int s21::Gauss::random100() {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(-9, 9);
  return (static_cast<int>(dis(gen)));
}
