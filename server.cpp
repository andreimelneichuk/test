#include <iostream>
#include <array>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define CYCLIC_BUFFER_SIZE 1024

class CyclicBuffer {
public:
    CyclicBuffer() : start(0), end(0), size(0) {}

    void addData(const std::string& data) {
        for (char c : data) {
            buffer[end] = c;
            end = (end + 1) % CYCLIC_BUFFER_SIZE;
            if (size < CYCLIC_BUFFER_SIZE) {
                size++;
            } else {
                start = (start + 1) % CYCLIC_BUFFER_SIZE; // циклический сдвиг
            }
        }
    }

    std::string getData() {
        std::string data;
        size_t current = start;
        for (size_t i = 0; i < size; ++i) {
            data += buffer[current];
            current = (current + 1) % CYCLIC_BUFFER_SIZE;
        }
        return data;
    }

private:
    std::array<char, CYCLIC_BUFFER_SIZE> buffer;
    size_t start;
    size_t end;
    size_t size;
};

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    CyclicBuffer cyclicBuffer;

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ошибка при создании сокета");
        return -1;
    }

    // Устанавливаем опцию для повторного использования адреса
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Привязка сокета к адресу и порту
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Ошибка привязки");
        close(server_fd);
        return -1;
    }

    // Ожидание входящих соединений
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка при прослушивании");
        close(server_fd);
        return -1;
    }

    std::cout << "Сервер запущен. Ожидание соединений..." << std::endl;

    while (true) {
        // Принимаем соединение
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Ошибка при принятии соединения");
            continue;
        }

        std::cout << "Новое соединение принято" << std::endl;

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        // Читаем данные из сокета
        while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            std::string data(buffer, bytes_read);
            cyclicBuffer.addData(data);

            std::cout << "Получено сообщение: " << data << std::endl;

            // Отправляем подтверждение
            std::string ack = "Сообщение получено";
            send(client_socket, ack.c_str(), ack.length(), 0);
        }

        if (bytes_read == 0) {
            std::cout << "Соединение закрыто" << std::endl;
        } else if (bytes_read < 0) {
            perror("Ошибка при чтении данных");
        }

        close(client_socket); // Закрываем соединение после обработки
    }

    close(server_fd); // Закрываем сокет сервера при завершении

    return 0;
}
