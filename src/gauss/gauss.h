#ifndef A3_PARALLELS_SRC_GAUSS_GAUSS_H_
#define A3_PARALLELS_SRC_GAUSS_GAUSS_H_

#include <fstream>
#include <random>
#include <thread>
#include <vector>

using namespace std;
namespace s21 {

class Gauss {
 public:
  vector<double> gauss(bool use_threads = false);
  vector<double> gauss_paralell(int count_iter);
  void loadGaussFromFile(string &filename);
  void loadRandmGauss(int size);
  int size();
  bool empty();

 private:
  vector<vector<double>> matrix_;
  vector<double> vector_;
  int size_{0};
  void resize(int size);
  void parallel_for(int k, vector<vector<double>> &m, vector<double> &v);
  void upTopMaxElement(int k, vector<vector<double>> &m, vector<double> &v);
  int random100();
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_GAUSS_GAUSS_H_
