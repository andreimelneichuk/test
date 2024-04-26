#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// Общая переменная для потоков чтения и записи
int shared_value = 0;

// Мьютекс для синхронизации доступа к общей переменной
std::mutex mtx;

// Функция для потоков записи
void writer(int writer_id) {
    int value = writer_id * 100; // Начальное значение зависит от id писателя
    while (true) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            shared_value = value++;
            std::cout << "Писатель " << writer_id << " записал: " << shared_value << std::endl;
        }

        // Пауза перед следующей записью
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void reader(int reader_id) {
    while (true) {
        int read_value;

        {
            // Блокируем мьютекс перед чтением
            std::lock_guard<std::mutex> lock(mtx);

            read_value = shared_value;
            std::cout << "Читатель " << reader_id << " прочитал: " << read_value << std::endl;
        }

        

        // Пауза перед следующим чтением
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    unsigned int num_threads = std::thread::hardware_concurrency();
    unsigned int num_writers = num_threads / 2; // Половина потоков будет писателями
    unsigned int num_readers = num_threads - num_writers; // Остальные будут читателями

    std::vector<std::thread> writer_threads;
    std::vector<std::thread> reader_threads;

    for (unsigned int i = 0; i < num_writers; i++) {
        writer_threads.emplace_back(writer, i + 1);
    }

    for (unsigned int i = 0; i < num_readers; i++) {
        reader_threads.emplace_back(reader, i + 1);
    }

    for (auto& t : writer_threads) {
        t.join();
    }

    for (auto& t : reader_threads) {
        t.join();
    }

    return 0;
}
