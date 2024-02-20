#ifndef A3_PARALLELS_SRC_SUPPORT_S21_SIMPLE_TIMER_H_
#define A3_PARALLELS_SRC_SUPPORT_S21_SIMPLE_TIMER_H_

#include <chrono>
#include <iostream>
namespace s21 {
class SimpleTimer {
 public:
  void start();
  void stop(const std::string &str = "");
  float stopReturn();

 private:
  std::chrono::time_point<std::chrono::steady_clock> begin, end;
  std::chrono::duration<float> duration;  //продолжительность
};

inline void SimpleTimer::start() { begin = std::chrono::steady_clock::now(); }

inline void SimpleTimer::stop(const std::string &str) {
  end = std::chrono::steady_clock::now();
  duration = end - begin;
  float result = duration.count();
  std::cout << str << "\t" << result * 1000 << " миллисекунд" << std::endl;
}

inline float SimpleTimer::stopReturn() {
  end = std::chrono::steady_clock::now();
  duration = end - begin;
  float result = duration.count();
  return result * 1000;
}

}  // namespace s21

#endif  // A3_PARALLELS_SRC_SUPPORT_S21_SIMPLE_TIMER_H_
