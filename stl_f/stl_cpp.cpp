#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Student {
private:
    string name;
    vector<int> grades;

public:
    Student() = default;

    Student(const string& name, const vector<int>& grades)
        : name(name), grades(grades) {}

    ~Student() = default;

    const string& getName() const {
        return name;
    }

    const vector<int>& getGrades() const {
        return grades;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setGrades(const vector<int>& newGrades) {
        grades = newGrades;
    }

    void addGrade(int grade) {
        if (grade < 1 || grade > 12) {
            throw out_of_range("Оценка должна быть от 1 до 12");
        }
        grades.push_back(grade);
    }

    double getAverageGrade() const {
        if (grades.empty()) {
            return 0.0;
        }

        int sum = 0;
        for (int grade : grades) {
            sum += grade;
        }
        return static_cast<double>(sum) / grades.size();
    }

    void printInfo() const {
        cout << "Имя: " << name << " | Оценки: ";

        if (grades.empty()) {
            cout << "нет оценок";
        } else {
            for (size_t i = 0; i < grades.size(); ++i) {
                cout << grades[i];
                if (i + 1 < grades.size()) {
                    cout << ", ";
                }
            }
            cout << " | Средняя: " << fixed << setprecision(2)
                 << getAverageGrade();
        }
        cout << '\n';
    }
};

class Group {
private:
    string groupName;
    vector<Student> students;

public:
    Group() = default;

    explicit Group(const string& groupName) : groupName(groupName) {}

    Group(const string& groupName, const vector<Student>& students)
        : groupName(groupName), students(students) {}

    ~Group() = default;

    const string& getGroupName() const {
        return groupName;
    }

    const vector<Student>& getStudents() const {
        return students;
    }

    size_t getStudentCount() const {
        return students.size();
    }

    void setGroupName(const string& newGroupName) {
        groupName = newGroupName;
    }

    void setStudents(const vector<Student>& newStudents) {
        students = newStudents;
    }

    void addStudent(const Student& student) {
        students.push_back(student);
    }

    bool removeStudent(size_t index) {
        if (index >= students.size()) {
            return false;
        }
        students.erase(students.begin() + index);
        return true;
    }

    void printStudents() const {
        cout << "\nГруппа: " << groupName << '\n';

        if (students.empty()) {
            cout << "В группе пока нет студентов.\n";
            return;
        }

        for (size_t i = 0; i < students.size(); ++i) {
            cout << i + 1 << ". ";
            students[i].printInfo();
        }
    }
};

int readInteger(const string& message) {
    int value;

    while (true) {
        cout << message;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Ошибка: введите целое число.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& message) {
    string text;
    do {
        cout << message;
        getline(cin, text);
        if (text.empty()) {
            cout << "Строка не должна быть пустой.\n";
        }
    } while (text.empty());
    return text;
}

Student createStudent() {
    string name = readText("Введите имя студента: ");
    int gradeCount;

    do {
        gradeCount = readInteger("Введите количество оценок: ");
        if (gradeCount < 0) {
            cout << "Количество не может быть отрицательным.\n";
        }
    } while (gradeCount < 0);

    vector<int> grades;
    for (int i = 0; i < gradeCount; ++i) {
        int grade;
        do {
            grade = readInteger("Оценка " + to_string(i + 1) + " (1-12): ");
            if (grade < 1 || grade > 12) {
                cout << "Оценка должна быть от 1 до 12.\n";
            }
        } while (grade < 1 || grade > 12);

        grades.push_back(grade);
    }

    return Student(name, grades);
}

int main() {
    Group group;
    bool groupCreated = false;

    while (true) {
        cout << "\n=== Работа с группой ===\n"
             << "1. Создать новую группу\n"
             << "2. Добавить студента\n"
             << "3. Удалить студента\n"
             << "4. Показать студентов\n"
             << "5. Выход\n";

        int choice = readInteger("Выберите пункт: ");

        if (choice == 1) {
            string name = readText("Введите название группы: ");
            group = Group(name);
            groupCreated = true;
            cout << "Группа создана.\n";
        } else if (choice == 2) {
            if (!groupCreated) {
                cout << "Сначала создайте группу.\n";
                continue;
            }
            group.addStudent(createStudent());
            cout << "Студент добавлен.\n";
        } else if (choice == 3) {
            if (!groupCreated) {
                cout << "Сначала создайте группу.\n";
                continue;
            }

            group.printStudents();
            if (group.getStudentCount() == 0) {
                continue;
            }

            int number = readInteger("Введите номер студента для удаления: ");
            if (number < 1 || !group.removeStudent(static_cast<size_t>(number - 1))) {
                cout << "Студента с таким номером нет.\n";
            } else {
                cout << "Студент удалён.\n";
            }
        } else if (choice == 4) {
            if (!groupCreated) {
                cout << "Сначала создайте группу.\n";
            } else {
                group.printStudents();
            }
        } else if (choice == 5) {
            cout << "До свидания!\n";
            break;
        } else {
            cout << "Выберите пункт от 1 до 5.\n";
        }
    }

}
