#ifndef A3_PARALLELS_SRC_ANT_ANT_COLONY_H_
#define A3_PARALLELS_SRC_ANT_ANT_COLONY_H_

#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "../support/s21_graph.h"
using namespace std;
namespace s21 {

struct TsmResult {
  vector<int> vertices;
  double distance = 0;
};
class AntColony {
 public:
  s21::TsmResult findPath(const s21::Graph &graph, int iteration = 1,
                          bool use_threads = false);

 private:
  struct Ant {
    vector<int> path;
    double distance;

    explicit Ant(int number_of_cities)
        : path(number_of_cities + 1, 0), distance(0) {}
  };

  const double kAlpha = 1;
  const double kBeta = 1;
  const double kStart_pheromone = 10;
  const int kQ = 30;
  const double kEvaporation = 0.2;

  std::mutex m;

  void initPheromone(const s21::Graph &graph, s21::Matrix &pheromone);
  void simulateAnts(const s21::Graph &graph, vector<Ant> &ant_colony,
                    const s21::Matrix &pheromone);
  void parallel_for(int count_iter, const s21::Graph &graph,
                    s21::Matrix &pheromone, s21::TsmResult &best_path);
  void updatePheromone(const vector<Ant> &ant_colony, s21::Matrix &pheromone);
  void updateBestPath(const vector<Ant> &ant_colony, s21::TsmResult &best_path);
  int randomCity(const s21::Graph &graph);
  double randomPercent();
};

}  // namespace s21

#endif  // A3_PARALLELS_SRC_ANT_ANT_COLONY_H_
