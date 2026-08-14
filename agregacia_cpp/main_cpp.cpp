#include <iostream>
#include <string>

using namespace std;

// 1. АГРЕГАЦИЯ
class Teacher {
private:
    string name;

public:
    explicit Teacher(const string& name) : name(name) {}

    const string& getName() const {
        return name;
    }
};

class University {
private:
    string name;
    Teacher* teacher; 

public:
    University(const string& name, Teacher* teacher)
        : name(name), teacher(teacher) {}

    void showInfo() const {
        cout << "Университет: " << name << '\n';
        if (teacher != nullptr) {
            cout << "Преподаватель: " << teacher->getName() << '\n';
        }
    }
};

// 2. КОМПОЗИЦИЯ
class Engine {
private:
    int power;

public:
    explicit Engine(int power) : power(power) {}

    int getPower() const {
        return power;
    }

    void start() const {
        cout << "Двигатель запущен.\n";
    }
};

class Car {
private:
    string model;
    Engine engine; 

public:
    Car(const string& model, int enginePower)
        : model(model), engine(enginePower) {}

    void showInfo() const {
        cout << "Автомобиль: " << model << '\n';
        cout << "Мощность двигателя: " << engine.getPower() << " л.с.\n";
        engine.start();
    }
};

// 3. НАСЛЕДОВАНИЕ
class Animal {
protected:
    string name;

public:
    explicit Animal(const string& name) : name(name) {}

    void eat() const {
        cout << name << " ест.\n";
    }
};

class Dog : public Animal {
public:
    explicit Dog(const string& name) : Animal(name) {}

    void bark() const {
        cout << name << " говорит: Гав!\n";
    }
};

int main() {
    cout << "Пример агрегации";
    Teacher teacher("Анна Петрова");
    University university("Компьютерная академия", &teacher);
    university.showInfo();

    cout << "Пример композиции";
    Car car("Toyota", 150);
    car.showInfo();

    cout << "Пример наследования ";
    Dog dog("Бим");
    dog.eat();  // Метод получен от базового класса Animal
    dog.bark(); // Собственный метод класса Dog
}
