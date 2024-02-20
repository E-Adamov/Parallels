#ifndef A3_PARALLELS_SRC_SUPPORT_S21_SCREEN_H_
#define A3_PARALLELS_SRC_SUPPORT_S21_SCREEN_H_

#include <iostream>
namespace s21 {
class Screen {
 public:
  static void clear();
};

inline void Screen::clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined(__APPLE__)
  system("clear");
#endif
}

}  // namespace s21

#endif  // A3_PARALLELS_SRC_SUPPORT_S21_SCREEN_H_
