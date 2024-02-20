#include "ant_colony.h"

s21::TsmResult s21::AntColony::findPath(const Graph &graph, int iteration,
                                        bool use_threads) {
  if (iteration <= 0) {
    throw std::invalid_argument("Некорректное число");
  }
  s21::TsmResult best_path{{}, INFINITY};
  const size_t number_of_cities = graph.getSize();
  s21::Matrix pheromone(number_of_cities, number_of_cities);
  initPheromone(graph, pheromone);

  if (use_threads) {
    parallel_for(iteration, graph, pheromone, best_path);
  } else {
    for (int i = 0; i < iteration; ++i) {
      vector<Ant> ant_colony(number_of_cities, Ant(number_of_cities));
      simulateAnts(graph, ant_colony, pheromone);
      updatePheromone(ant_colony, pheromone);
      updateBestPath(ant_colony, best_path);
    }
  }
  if (best_path.distance == INFINITY)
    throw runtime_error("Невозможно найти решение");
  return best_path;
}

void s21::AntColony::initPheromone(const Graph &graph, Matrix &pheromone) {
  for (size_t i = 0; i < graph.getSize(); ++i) {
    for (size_t j = 0; j < graph.getSize(); ++j) {
      pheromone(i, j) = kStart_pheromone;
    }
  }
}

void s21::AntColony::simulateAnts(const Graph &graph, vector<Ant> &ant_colony,
                                  const Matrix &pheromone) {
  for (auto &ant : ant_colony) {
    int current_city;
    const size_t number_of_cities = graph.getSize();
    vector<bool> visited(number_of_cities, false);
    current_city = randomCity(graph);
    ant.path[0] = current_city;
    ant.distance = 0;

    for (size_t step = 1; step < number_of_cities; ++step) {
      visited[current_city] = true;
      double atraction_sum = 0;

      vector<double> cities_attraction(number_of_cities, 0);
      for (size_t city_to = 0; city_to < number_of_cities; ++city_to) {
        double distance = (double)graph(current_city, city_to);
        if (!visited[city_to] && distance != 0) {
          cities_attraction[city_to] =
              pow(pheromone(current_city, city_to), kAlpha) *
              pow((1.0 / distance), kBeta);
        }

        atraction_sum += cities_attraction[city_to];
      }

      if (atraction_sum != 0) {
        cities_attraction[0] = cities_attraction[0] / atraction_sum;
        for (size_t c = 1; c < number_of_cities; ++c)
          cities_attraction[c] =
              cities_attraction[c - 1] + cities_attraction[c] / atraction_sum;

        double ant_choise = randomPercent();
        int city_to = 0;
        while (ant_choise > cities_attraction[city_to]) ++city_to;

        ant.distance += graph(current_city, city_to);

        ant.path[step] = city_to;
        current_city = city_to;

      } else {
        ant.distance = INFINITY;
        break;
      }
    }

    int back_path = graph(ant.path[0], ant.path[number_of_cities - 1]);
    if (back_path != 0) {
      ant.distance += back_path;
      ant.path[number_of_cities] = ant.path[0];
    } else {
      ant.distance = INFINITY;
    }
  }
}

void s21::AntColony::parallel_for(int count_iter, const Graph &graph,
                                  Matrix &pheromone, TsmResult &best_path) {
  int input_threads = std::thread::hardware_concurrency();
  input_threads = input_threads == 0 ? 2 : input_threads;
  int num_threads = count_iter < input_threads ? count_iter : input_threads;
  int shift_thread = count_iter / num_threads;
  std::vector<std::thread> threads(num_threads);

  auto one_thread = [&](int begin, int end) {
    for (auto it = begin; it != end; ++it) {
      vector<Ant> ant_colony(graph.getSize(), Ant(graph.getSize()));
      simulateAnts(graph, ant_colony, pheromone);
      m.lock();
      updatePheromone(ant_colony, pheromone);
      updateBestPath(ant_colony, best_path);
      m.unlock();
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

void s21::AntColony::updatePheromone(const vector<Ant> &ant_colony,
                                     Matrix &pheromone) {
  for (size_t i = 0; i < pheromone.getNumRows(); ++i)
    for (size_t j = 0; j < pheromone.getNumColumns(); ++j)
      pheromone(i, j) = (1 - kEvaporation) * pheromone(i, j);

  for (Ant ant : ant_colony) {
    double pheromone_add = kQ / ant.distance;

    for (size_t city = 1; city < ant.path.size(); ++city) {
      int to = ant.path[city];
      int from = ant.path[city - 1];
      pheromone(from, to) += pheromone_add;
      pheromone(to, from) += pheromone_add;
    }
  }
}

void s21::AntColony::updateBestPath(const vector<Ant> &ant_colony,
                                    TsmResult &best_path) {
  for (Ant ant : ant_colony) {
    if (ant.distance < best_path.distance) {
      best_path.vertices = ant.path;
      best_path.distance = ant.distance;
    }
  }
}

int s21::AntColony::randomCity(const Graph &graph) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, graph.getSize() - 1);
  return (static_cast<double>(dis(gen)));
}

double s21::AntColony::randomPercent() {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0.0, 1.0);
  return (static_cast<double>(dis(gen)));
}
