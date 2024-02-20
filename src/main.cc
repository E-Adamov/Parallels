#include <iostream>
#include "support/s21_screen.h"
#include "ant/ant_interface.h"
#include "gauss/gauss_interface.h"
#include "winograd/winograd_interface.h"

int main() {
  cout << endl;
  s21::Screen::clear();
  string input;
  while (true) {
    cout << endl
         << "------------------------A3_Parallels-------------------------"
         << endl;
    cout << "Выберите пункт:" << endl;
    cout << "1. Part 1. Муравьиный алгоритм" << endl;
    cout << "2. Part 2. Решение СЛАУ" << endl;
    cout << "3. Part 3. Алгоритм Винограда" << endl;
    cout << "4. Выход из программы" << endl;
    cout << "-------------------------------------------------------------"
         << endl;

    cout << "Введите от 1 до 4: ";
    cin >> input;
    if (input >= "1" && input <= "3" && input.size() == 1) {
      if (input == "1") {
        s21::AntInterface ai;
        ai.start();
      } else if (input == "2") {
        s21::GaussInterface gi;
        gi.start();
      } else if (input == "3") {
        s21::WinogradInterface wi;
        wi.start();
      }
    } else if (input == "4") {
      break;
    } else {
      cout << "\nОШИБКА: Некорректный ввод" << endl;
    }
  }
}
