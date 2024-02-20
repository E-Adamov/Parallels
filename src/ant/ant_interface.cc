#include "ant_interface.h"

void s21::AntInterface::start() {
  s21::Screen::clear();
  string input;
  while (true) {
    cout << endl
         << "-----------------Part 1. Муравьиный алгоритм-----------------"
         << endl;
    cout << "Выберите пункт:" << endl;
    cout << "1. Загрузка исходного графа из файла" << endl;
    cout << "2. Решение задачи комивояжера" << endl;
    cout << "3. Выход из программы" << endl;
    cout << "-------------------------------------------------------------"
         << endl;

    cout << "Введите от 1 до 3: ";
    cin >> input;
    s21::Screen::clear();
    if (input >= "1" && input <= "2" && input.size() == 1) {
      if (graph_.getSize() == 0 && input >= "2" && input <= "8") {
        cout << "\nОШИБКА: Граф пустой" << endl;
      } else if (input == "1") {
        try {
          cout << "\nУкажите название файла для загрузки:" << endl;
          cout << "Например: v5.txt, v6.txt, v11.txt" << endl;
          string file_path;
          cin >> file_path;
          graph_.loadGraphFromFile(file_path);
          cout << "Граф успешно загружен" << endl;
        } catch (const exception &e) {
          cout << "ОШИБКА: ";
          cout << e.what() << endl;
        }
      } else if (input == "2") {
        cout << "\nРешение задачи комивояжера" << endl;
        cout << "Введите количество итераций:" << endl;
        s21::AntColony ac;
        string str;
        cin >> str;
        try {
          int count_iter = stoi(str);
          try {
            s21::SimpleTimer timer_thread, timer_not_thread;
            timer_thread.start();
            auto tsm_thread = ac.findPath(graph_, count_iter, true);
            cout << "\nВремя, для выполнения " << count_iter
                 << " раз муравьиного алгоритма с применением параллелизма:"
                 << endl;
            cout << timer_thread.stopReturn() << " миллисекунд" << endl;
            cout << "Дистанция: " << endl;
            cout << tsm_thread.distance << endl;
            cout << "Полученный путь:" << endl;
            for (auto &it : tsm_thread.vertices) {
              cout << it << " ";
            }
            cout << endl;

            timer_not_thread.start();
            auto tsm = ac.findPath(graph_, count_iter);
            cout << "\nВремя, для выполнения " << count_iter
                 << " раз обычного муравьиного алгоритма:" << endl;
            cout << timer_not_thread.stopReturn() << " миллисекунд" << endl;
            cout << "Дистанция: " << endl;
            cout << tsm.distance << endl;
            cout << "Полученный путь:" << endl;
            for (auto &it : tsm.vertices) {
              cout << it << " ";
            }
            cout << endl;
          } catch (const exception &e) {
            cout << "ОШИБКА: ";
            cout << e.what() << endl;
          }
        } catch (...) {
          cout << "ОШИБКА: Число не подходит, введите другое число" << endl;
        }
      }

    } else if (input == "3") {
      break;
    } else {
      cout << "\nОШИБКА: Некорректный ввод" << endl;
    }
  }
}
