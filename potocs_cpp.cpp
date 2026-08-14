#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>

using namespace std;

class GameHistory {
private:
    string file_name;

public:
    explicit GameHistory(const string& file_name) : file_name(file_name) {}

    void save_game(int secret_number, int attempts) const {
        // ios::app добавляет запись в конец, не удаляя предыдущие игры.
        ofstream file(file_name, ios::app);

        if (!file.is_open()) {
            cerr << "Не удалось открыть файл для записи.\n";
            return;
        }

        time_t current_time = time(nullptr);
        tm* local_time = std::localtime(&current_time);

        file << put_time(local_time, "%Y-%m-%d %H:%M:%S") << ';'
             << secret_number << ';' << attempts << '\n';
    }

    void show_statistics() const {
        ifstream file(file_name);

        if (!file.is_open()) {
            cout << "История игр пока пуста.\n";
            return;
        }

        string line;
        int games_count = 0;
        int total_attempts = 0;
        int best_result = numeric_limits<int>::max();

        cout << "\nИстория игр:\n";
        cout << left << setw(6) << "Игра"
             << setw(22) << "Дата"
             << setw(18) << "Число"
             << "Попытки\n";

        while (getline(file, line)) {
            string date;
            string number_text;
            string attempts_text;
            stringstream parser(line);

            if (!getline(parser, date, ';') ||
                !getline(parser, number_text, ';') ||
                !getline(parser, attempts_text)) {
                continue;
            }

            try {
                int secret_number = stoi(number_text);
                int attempts = stoi(attempts_text);

                ++games_count;
                total_attempts += attempts;
                if (attempts < best_result) {
                    best_result = attempts;
                }

                cout << left << setw(6) << games_count
                     << setw(22) << date
                     << setw(18) << secret_number
                     << attempts << '\n';
            } catch (...) {
            }
        }

        if (games_count == 0) {
            cout << "Сохранённых игр нет.\n";
            return;
        }

        cout << "\nВсего игр: " << games_count << '\n';
        cout << "Лучший результат: " << best_result << " попыток\n";
        cout << fixed << setprecision(1)
             << "Среднее количество попыток: "
             << static_cast<double>(total_attempts) / games_count << "\n";
    }
};

class GuessNumberGame {
private:
    GameHistory history;
    mt19937 generator;

    static int read_integer(const string& message) {
        int value;

        while (true) {
            cout << message;
            if (cin >> value) {
                return value;
            }

            cout << "Ошибка: введите целое число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void play() {
        uniform_int_distribution<int> distribution(1, 500);
        int secret_number = distribution(generator);
        int attempts = 0;

        cout << "\nЯ загадал число от 1 до 500. Попробуйте угадать!\n";

        while (true) {
            int guess = read_integer("Ваш вариант: ");

            if (guess < 1 || guess > 500) {
                cout << "Введите число от 1 до 500.\n";
                continue;
            }

            ++attempts;

            if (guess < secret_number) {
                cout << "Загаданное число больше.\n";
            } else if (guess > secret_number) {
                cout << "Загаданное число меньше.\n";
            } else {
                cout << "Вы угадали число за " << attempts << " попыток!\n";
                history.save_game(secret_number, attempts);
                break;
            }
        }
    }

public:
    GuessNumberGame()
        : history("game_history.txt"), generator(random_device{}()) {}

    void run() {
        while (true) {
            cout << "\n=== Игра Вгадай число ===\n"
                 << "1. Начать новую игру\n"
                 << "2. Показать статистику\n"
                 << "3. Выход\n";

            int choice = read_integer("Выберите пункт: ");

            switch (choice) {
                case 1:
                    play();
                    break;
                case 2:
                    history.show_statistics();
                    break;
                case 3:
                    cout << "До свидания!\n";
                    return;
                default:
                    cout << "Выберите пункт от 1 до 3.\n";
            }
        }
    }
};

int main() {
    GuessNumberGame game;
    game.run();
}
