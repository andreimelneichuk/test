# test

Небольшие учебные упражнения на C++. Каждый файл — отдельная программа со своим `main`.

| Файл | Что делает |
| :--- | :--- |
| `array.cpp` | Доступ к элементу массива через арифметику указателей |
| `database.cpp` | In-memory «база» студентов на `std::unordered_map` и `std::shared_ptr`: добавление, удаление, поиск, проверка ID с исключениями |
| `server.cpp` | TCP-сервер на POSIX-сокетах (порт 8080): принимает сообщения, складывает их в циклический буфер на 1024 байта и отвечает подтверждением |
| `thread.cpp` | Потоки-писатели и потоки-читатели (поровну от `hardware_concurrency()`) с общей переменной под `std::mutex` |

## Сборка

```bash
g++ -std=c++17 -o array array.cpp
g++ -std=c++17 -o database database.cpp
g++ -std=c++17 -o server server.cpp
g++ -std=c++17 -pthread -o thread thread.cpp
```
