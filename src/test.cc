#include <gtest/gtest.h>

#include "ant/ant_colony.h"
#include "gauss/gauss.h"
#include "winograd/winograd.h"
#include <vector>
using namespace std;

const double eps = 1e-6;

TEST(ant, one_thread) {
  s21::Graph graph;
  string file_path = "examples/v5.txt";
  int count_iter = 100;
  int test_distance = 11;
  graph.loadGraphFromFile(file_path);
  s21::AntColony ac;
  auto result = ac.findPath(graph);
  EXPECT_EQ(result.distance, test_distance);
}
TEST(ant, multi_threads) {
  s21::Graph graph;
  string file_path = "examples/v5.txt";
  int count_iter = 100;
  int test_distance = 11;
  graph.loadGraphFromFile(file_path);
  s21::AntColony ac;
  auto result = ac.findPath(graph, count_iter, true);
  EXPECT_EQ(result.distance, test_distance);
}

TEST(gauss, empty) {
  s21::Gauss gauss;
  EXPECT_EQ(gauss.empty(), true);
}

TEST(gauss, size) {
  s21::Gauss gauss;
  int random_size = 3;
  EXPECT_EQ(gauss.size(), 0);
  gauss.loadRandmGauss(random_size);
  EXPECT_EQ(gauss.size(), random_size);
}

TEST(gauss, random) {
  s21::Gauss gauss;
  int random_size = 3;
  gauss.loadRandmGauss(random_size);
  auto result = gauss.gauss();
  EXPECT_EQ(result.size(), random_size);
}

TEST(gauss, one_thread) {
  s21::Gauss gauss;
  vector<double> test = {-1, 3, 1};
  string file_path("examples/g3.txt");
  gauss.loadGaussFromFile(file_path);
  auto result = gauss.gauss();
  EXPECT_EQ(result.size(), test.size());
  for (int i = 0; i < test.size(); ++i)
    EXPECT_NEAR(result[i], test[i], eps);
}

TEST(gauss, multi_thread1) {
  s21::Gauss gauss;
  vector<double> test = {-1, 3, 1};
  string file_path("examples/g3.txt");
  gauss.loadGaussFromFile(file_path);
  auto result = gauss.gauss(true);
  EXPECT_EQ(result.size(), test.size());
  for (int i = 0; i < test.size(); ++i)
    EXPECT_NEAR(result[i], test[i], eps);
}

TEST(gauss, multi_thread2) {
  s21::Gauss gauss;
  int count_iter = 10;
  vector<double> test = {-1, 3, 1};
  string file_path("examples/g3.txt");
  gauss.loadGaussFromFile(file_path);
  auto result = gauss.gauss_paralell(count_iter);
  EXPECT_EQ(result.size(), test.size());
  for (int i = 0; i < test.size(); ++i)
    EXPECT_NEAR(result[i], test[i], eps);
}

TEST(winograd, random) {
  s21::Winograd winograd;
  vector<vector<int>> a, b, res, res1;
  int a_rows = 3, a_cols = 3, b_rows = 3, b_cols = 4;

  winograd.initMatrix(a, a_rows, a_cols);
  winograd.fillMatrix(a);
  winograd.initMatrix(b, b_rows, b_cols);
  winograd.fillMatrix(b);

  res = winograd.multWinograd(a, b);
  EXPECT_EQ(res.size(), a_rows);
  EXPECT_EQ(res[0].size(), b_cols);
}

TEST(winograd, mult) {
  s21::Winograd winograd;
  vector<vector<int>> a, b, res;
  string file_path("examples/m2.txt");

  vector<vector<int>> test = {
      {14, 4, 2, -2}, {-34, -5, -16, 4}, {40, 13, 2, -6}};

  winograd.loadMatrixFromFile(file_path, a, b);
  res = winograd.multMatrix(a, b);
  for (int r = 0; r < test.size(); ++r)
    for (int c = 0; c < test.size(); ++c)
      EXPECT_EQ(res[r][c], test[r][c]);
}

TEST(winograd, multWinograd_one_thread) {
  s21::Winograd winograd;
  vector<vector<int>> a, b, res;
  string file_path("examples/m2.txt");

  vector<vector<int>> test = {
      {14, 4, 2, -2}, {-34, -5, -16, 4}, {40, 13, 2, -6}};

  winograd.loadMatrixFromFile(file_path, a, b);
  res = winograd.multWinograd(a, b);
  for (int r = 0; r < test.size(); ++r)
    for (int c = 0; c < test.size(); ++c)
      EXPECT_EQ(res[r][c], test[r][c]);
}

TEST(winograd, multWinograd_multi_threads) {
  s21::Winograd winograd;
  vector<vector<int>> a, b, res;
  string file_path("examples/m2.txt");
  int count_iter = 10;

  vector<vector<int>> test = {
      {14, 4, 2, -2}, {-34, -5, -16, 4}, {40, 13, 2, -6}};

  winograd.loadMatrixFromFile(file_path, a, b);
  res = winograd.multWinograd(a, b, true, count_iter);
  for (int r = 0; r < test.size(); ++r)
    for (int c = 0; c < test.size(); ++c)
      EXPECT_EQ(res[r][c], test[r][c]);
}

TEST(winograd, pipelineWinograd) {
  s21::Winograd winograd;
  vector<vector<int>> a, b, res;
  string file_path("examples/m2.txt");

  vector<vector<int>> test = {
      {14, 4, 2, -2}, {-34, -5, -16, 4}, {40, 13, 2, -6}};

  winograd.loadMatrixFromFile(file_path, a, b);
  res = winograd.pipelineWinograd(a, b);
  for (int r = 0; r < test.size(); ++r)
    for (int c = 0; c < test.size(); ++c)
      EXPECT_EQ(res[r][c], test[r][c]);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}