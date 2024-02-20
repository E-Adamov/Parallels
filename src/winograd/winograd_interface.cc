#include "winograd_interface.h"

void s21::WinogradInterface::start() {
  s21::Screen::clear();
  string input;
  while (true) {
    cout << endl
         << "-----------------Part 3. Алгоритм Винограда------------------"
         << endl;
    cout << "Выберите пункт:" << endl;
    cout << "1. Настройки вывода:" << endl;
    cout << "\tВывод матриц: " << (print_matrix == true ? "да" : "нет") << endl;
    cout << "\tОбычное умножение: " << (mult == true ? "да" : "нет") << endl;
    cout << "\tАлгоритм Винограда: " << (mult_vinograd == true ? "да" : "нет")
         << endl;
    cout << "\tКлассический параллелизм: " << (classic == true ? "да" : "нет")
         << endl;
    cout << "\tКонвейерный параллелизм: " << (pipeline == true ? "да" : "нет")
         << endl;
    cout << "2. Количество итераций: " << count_iter << endl;
    cout << "3. Размер случайных матриц" << endl;
    cout << "\tМатрица_A: " << a_rows << " x " << a_cols << endl;
    cout << "\tМатрица_B: " << b_rows << " x " << b_cols << endl;
    cout << "4. Умножение случайных матриц" << endl;
    cout << "5. Умножение введенных матриц" << endl;
    cout << "6. Выход из программы" << endl;
    cout << "-------------------------------------------------------------"
         << endl;

    cout << "Введите от 1 до 6: ";
    cin >> input;
    s21::Screen::clear();
    if (input >= "1" && input <= "5" && input.size() == 1) {
      //---------------------------------------
      if (input == "1") {
        while (true) {
          s21::Screen::clear();
          cout << "Настройки вывода:" << endl;
          cout << "1. Вывод матриц: " << (print_matrix == true ? "да" : "нет")
               << endl;
          cout << "2. Обычное умножение: " << (mult == true ? "да" : "нет")
               << endl;
          cout << "3. Алгоритм Винограда: "
               << (mult_vinograd == true ? "да" : "нет") << endl;
          cout << "4. Классический параллелизм: "
               << (classic == true ? "да" : "нет") << endl;
          cout << "5. Конвейерный параллелизм: "
               << (pipeline == true ? "да" : "нет") << endl;
          cout << "6. Выход из настройки" << endl;
          cout << endl;
          cout << "Введите от 1 до 6: ";
          string str;
          cin >> str;
          if (str >= "1" && str <= "5" && str.size() == 1) {
            if (str == "1") {
              print_matrix = !print_matrix;
            } else if (str == "2") {
              mult = !mult;
            } else if (str == "3") {
              mult_vinograd = !mult_vinograd;
            } else if (str == "4") {
              classic = !classic;
            } else if (str == "5") {
              pipeline = !pipeline;
            }
          } else if (str == "6") {
            break;
          } else {
            cout << "ОШИБКА: Число не подходит, введите другое число" << endl;
          }
        }
        //---------------------------------------
      } else if (input == "2") {
        cout << "Введите количество итераций:" << endl;
        string str;
        cin >> str;
        try {
          int temp_count_iter = stoi(str);
          if (temp_count_iter > 0) {
            count_iter = temp_count_iter;
          } else {
            throw 1;
          }
        } catch (...) {
          cout << "\nОШИБКА: Число не подходит, введите другое число" << endl;
        }
        //---------------------------------------
      } else if (input == "3") {
        cout << "Введите размер случайных матриц:" << endl;
        string str_a, str_b, str_c, str_d;

        cout << "Введите количество рядов матрицы А:" << endl;
        cin >> str_a;
        cout << "Введите количество строк матрицы А:" << endl;
        cin >> str_b;
        cout << "Введите количество рядов матрицы В:" << endl;
        cin >> str_c;
        cout << "Введите количество строк матрицы В:" << endl;
        cin >> str_d;
        cout << endl;
        try {
          int temp_a_rows = stoi(str_a);
          int temp_a_cols = stoi(str_b);
          int temp_b_rows = stoi(str_c);
          int temp_b_cols = stoi(str_d);
          try {
            if (temp_a_rows > 0 && temp_a_cols > 0 && temp_b_rows > 0 &&
                temp_b_cols > 0) {
              try {
                if (temp_a_cols == temp_b_rows) {
                  a_rows = temp_a_rows;
                  a_cols = temp_a_cols;
                  b_rows = temp_b_rows;
                  b_cols = temp_b_cols;
                } else {
                  throw 1;
                }
              } catch (...) {
                cout << "ОШИБКА: Кол-во столбцов не равно кол-ву строк" << endl;
              }
            } else {
              throw 1;
            }
          } catch (...) {
            cout << "ОШИБКА: Размеры матрицы должен быть больше нуля" << endl;
          }
        } catch (...) {
          cout << "ОШИБКА: Число не подходит, введите другое число" << endl;
        }
        //---------------------------------------
      } else if (input == "4") {
        matrix a, b;
        cout << "заполнение матрицы А... " << endl;
        w_.initMatrix(a, a_rows, a_cols);
        w_.fillMatrix(a);
        cout << "заполнение матрицы В... " << endl;
        w_.initMatrix(b, b_rows, b_cols);
        w_.fillMatrix(b);
        cout << endl;
        printResult(a, b);
        //---------------------------------------
      } else if (input == "5") {
        try {
          cout << "\nУкажите название файла для загрузки:" << endl;
          cout << "Например: m2.txt" << endl;
          matrix a, b;
          string file_path;
          cin >> file_path;
          w_.loadMatrixFromFile(file_path, a, b);
          cout << "матрицы успешно загружены" << endl;
          cout << endl;
          printResult(a, b);
        } catch (const exception &e) {
          cout << "ОШИБКА: ";
          cout << e.what() << endl;
        }
      }
    } else if (input == "6") {
      break;
    } else {
      cout << "\nОШИБКА: Некорректный ввод" << endl;
    }
  }
}

void s21::WinogradInterface::printMatrix(vector<vector<int>> &m) {
  std::cout << "----------------" << std::endl;

  for (size_t r = 0; r < m.size(); ++r) {
    for (size_t c = 0; c < m[r].size(); ++c) {
      std::cout << m[r][c] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "----------------" << std::endl;
}

void s21::WinogradInterface::printResult(matrix &a, matrix &b) {
  s21::SimpleTimer t1, t2, t3, t4;
  matrix c, d, e, h;

  if (print_matrix) {
    cout << "Матрица А:" << endl;
    printMatrix(a);
    cout << "Матрица В:" << endl;
    printMatrix(b);
  }
  cout << "подсчёт времени... " << endl;
  cout << endl;
  if (mult) {
    t1.start();
    for (size_t it = 0; it < count_iter; ++it) c = w_.multMatrix(a, b);
    t1.stop("Обычное умножение: ");
  }

  if (mult_vinograd) {
    t2.start();
    for (size_t it = 0; it < count_iter; ++it) d = w_.multWinograd(a, b, 0);
    t2.stop("Алгоритм Винограда: ");
  }

  if (classic) {
    cout << "Классический параллелизм: " << endl;
    int max_threads = std::thread::hardware_concurrency();
    cout << "Число логических процессоров компьютера: " << max_threads << endl;
    max_threads = max_threads == 0 ? 2 : max_threads;
    for (int i = 2; i <= 4 * max_threads; i = i * 2) {
      t3.start();
      for (size_t it = 0; it < count_iter; ++it)
        e = w_.multWinograd(a, b, 1, i);
      cout << "кол-во потоков " << i << " ";
      t3.stop(": ");
    }
  }

  if (pipeline) {
    t4.start();
    for (size_t it = 0; it < count_iter; ++it) h = w_.pipelineWinograd(a, b);
    t4.stop("Конвейерный паралл.: ");
  }

  cout << endl;
  if (print_matrix) {
    if (mult) {
      cout << "Результат Обычное умножение:" << endl;
      printMatrix(c);
    }
    if (mult_vinograd) {
      cout << "Результат Алгоритм Винограда:" << endl;
      printMatrix(d);
    }
    if (classic) {
      cout << "Результат Классический параллелизм:" << endl;
      printMatrix(e);
    }
    if (pipeline) {
      cout << "Результат Конвейерный параллелизм:" << endl;
      printMatrix(h);
    }
  }
}
