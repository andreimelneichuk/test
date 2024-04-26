#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cctype>

struct Student {
    int id;
    std::string name;
    int age;

    Student(int id, const std::string& name, int age)
        : id(id), name(name), age(age) {}
};

class StudentDatabase {
public:
    static int validateId(const std::string& id_str) {
        for (char c : id_str) {
            if (!std::isdigit(c)) {
                throw std::invalid_argument("Идентификатор должен быть типа int.");
            }
        }
        return std::stoi(id_str);
    }

    void addStudent(const std::string& id_str, const std::string& name, int age) {
        int id = validateId(id_str);

        if (students.find(id) != students.end()) {
            std::cerr << "Ошибка: Студент с ID " << id << " уже существует.\n";
            throw std::invalid_argument("Дубликат ID");
        }

        if (age <= 0) {
            std::cerr << "Ошибка: Возраст должен быть положительным.\n";
            throw std::invalid_argument("Некорректный возраст");
        }

        students[id] = std::make_shared<Student>(id, name, age);
        std::cout << "Студент " << name << " добавлен.\n";
    }

    void removeStudent(const std::string& id_str) {
        int id = validateId(id_str);

        if (students.erase(id) == 0) {
            std::cerr << "Ошибка: Студент с ID " << id << " не найден.\n";
            throw std::invalid_argument("Студент не найден");
        }
        std::cout << "Студент с ID " << id << " удален.\n";
    }

    std::shared_ptr<Student> getStudent(const std::string& id_str) {
        int id = validateId(id_str);

        auto it = students.find(id);
        if (it == students.end()) {
            std::cerr << "Ошибка: Студент с ID " << id << " не найден.\n";
            throw std::out_of_range("Студент не найден");
        }
        return it->second;
    }

private:
    std::unordered_map<int, std::shared_ptr<Student>> students;
};

int main() {
    StudentDatabase db;

    try {
        db.addStudent("1", "John Doe", 20);
        db.addStudent("2", "Jane Doe", 22);

        db.addStudent("1", "Duplicate", 25);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Исключение: " << e.what() << '\n';
    }

    try {
        auto student = db.getStudent("1");
        std::cout << "Студент с ID 1: " << student->name << ", возраст: " << student->age << '\n';

        db.removeStudent("1");

        db.getStudent("1");

    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << '\n';
    }
    return 0;
}
