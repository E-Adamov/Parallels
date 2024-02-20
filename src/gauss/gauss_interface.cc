#include "gauss_interface.h"

void s21::GaussInterface::start() {
  s21::Screen::clear();
  string input;

  while (true) {
    cout << endl
         << "--------------------Part 2. Решение СЛАУ---------------------"
         << endl;
    cout << "Выберите пункт:" << endl;
    cout << "1. Загрузка исходного СЛАУ из файла" << endl;
    cout << "2. Создать случайный СЛАУ" << endl;
    cout << "3. Решение СЛАУ размером " << gauss_.size() << " методом Гаусса"
         << endl;
    cout << "4. Выход из программы" << endl;
    cout << "-------------------------------------------------------------"
         << endl;

    cout << "Введите от 1 до 4: ";
    cin >> input;
    s21::Screen::clear();
    if (input >= "1" && input <= "3" && input.size() == 1) {
      if (gauss_.empty() && input >= "3" && input <= "3") {
        cout << "\nОШИБКА: СЛАУ пустой" << endl;
      } else if (input == "1") {
        try {
          cout << "\nУкажите название файла для загрузки:" << endl;
          cout << "Например: g3.txt, g41.txt, g42.txt" << endl;
          string file_path;
          cin >> file_path;
          gauss_.loadGaussFromFile(file_path);
          cout << "СЛАУ успешно загружен" << endl;
        } catch (const exception &e) {
          cout << "ОШИБКА: ";
          cout << e.what() << endl;
        }
      } else if (input == "2") {
        cout << "Введите размер СЛАУ (например: от 1 до 300):" << endl;
        string str;
        cin >> str;
        try {
          int size = stoi(str);
          gauss_.loadRandmGauss(size);
          cout << "СЛАУ успешно загружен" << endl;
        } catch (...) {
          cout << "ОШИБКА: Число не подходит, введите другое число" << endl;
        }
      } else if (input == "3") {
        cout << "\nРешение СЛАУ методом Гаусса" << endl;
        cout << "Введите количество итераций:" << endl;
        string str;
        cin >> str;
        try {
          int count_iter = stoi(str);
          try {
            s21::SimpleTimer timer_inside_thread, timer_outside_thread,
                timer_not_thread;
            timer_inside_thread.start();
            for (int i = 0; i < count_iter - 1; ++i) gauss_.gauss(true);
            auto res1 = gauss_.gauss(true);
            cout << "\nВремя, для выполнения " << count_iter
                 << " раз \nпараллельного СЛАУ алгоритма(зависит от размера "
                    "СЛАУ):"
                 << endl;
            printTime(timer_inside_thread.stopReturn());
            printResult(res1);
            cout << endl;

            timer_outside_thread.start();
            auto res2 = gauss_.gauss_paralell(count_iter);
            cout << "\nВремя, для выполнения " << count_iter
                 << " раз \nпараллельного СЛАУ алгоритма(зависит от "
                    "количества итераций):"
                 << endl;
            printTime(timer_outside_thread.stopReturn());
            printResult(res2);
            cout << endl;

            timer_not_thread.start();
            for (int i = 0; i < count_iter - 1; ++i) gauss_.gauss();
            auto res3 = gauss_.gauss();
            cout << "\nВремя, для выполнения " << count_iter
                 << " раз обычного СЛАУ алгоритма:" << endl;
            printTime(timer_not_thread.stopReturn());
            printResult(res3);
            cout << endl;
          } catch (const exception &e) {
            cout << "ОШИБКА: ";
            cout << e.what() << endl;
          }
        } catch (...) {
          cout << "ОШИБКА: Число не подходит, введите другое число" << endl;
        }
      }

    } else if (input == "4") {
      break;
    } else {
      cout << "\nОШИБКА: Некорректный ввод" << endl;
    }
  }
}

void s21::GaussInterface::printResult(const std::vector<double> &v) {
  if (gauss_.size() < 11) {
    cout << "Результат:" << endl;
    for (auto &it : v) {
      cout << fixed << setprecision(3) << it << " ";
    }
  }
}

void s21::GaussInterface::printTime(float time) {
  cout << fixed << setprecision(5) << time << " миллисекунд" << endl;
}
