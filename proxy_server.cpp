#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // Для getaddrinfo
#include <thread>
#include <vector>
#include <sstream>
#include <regex>

#define BUFFER_SIZE 4096

void log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

// Функция для извлечения хоста и порта из CONNECT-запроса
bool extract_host_and_port(const std::string& request, std::string& host, int& port) {
    std::regex connect_regex(R"(CONNECT\s+([^\s:]+):(\d+))");
    std::smatch match;

    if (std::regex_search(request, match, connect_regex)) {
        host = match[1].str();
        port = std::stoi(match[2].str());
        return true;
    }

    return false;
}

// Функция для установки TCP-соединения с целевым сервером
int connect_to_target(const std::string& host, int port) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Преобразуем доменное имя в IP-адрес
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
        log("Ошибка при разрешении доменного имени: " + host);
        return -1;
    }

    // Создаем сокет
    int server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_socket < 0) {
        log("Ошибка при создании сокета для сервера");
        freeaddrinfo(res);
        return -1;
    }

    // Устанавливаем соединение с целевым сервером
    if (connect(server_socket, res->ai_addr, res->ai_addrlen) < 0) {
        log("Ошибка при подключении к целевому серверу: " + host + ":" + std::to_string(port));
        close(server_socket);
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);
    return server_socket;
}

// Функция для обработки CONNECT-запроса
void handle_connect_request(int client_socket, const std::string& host, int port) {
    // Устанавливаем соединение с целевым сервером
    int server_socket = connect_to_target(host, port);
    if (server_socket < 0) {
        close(client_socket);
        return;
    }

    // Отправляем клиенту успешный ответ на CONNECT-запрос
    const char* response = "HTTP/1.1 200 Connection Established\r\n\r\n";
    send(client_socket, response, strlen(response), 0);

    // Туннелирование данных между клиентом и сервером
    fd_set read_fds;
    char buffer[BUFFER_SIZE];
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);
        FD_SET(server_socket, &read_fds);

        int max_fd = std::max(client_socket, server_socket);
        if (select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr) < 0) {
            log("Ошибка в select");
            break;
        }

        // Передача данных от клиента к серверу
        if (FD_ISSET(client_socket, &read_fds)) {
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;
            send(server_socket, buffer, bytes_received, 0);
        }

        // Передача данных от сервера к клиенту
        if (FD_ISSET(server_socket, &read_fds)) {
            ssize_t bytes_received = recv(server_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;
            send(client_socket, buffer, bytes_received, 0);
        }
    }

    close(client_socket);
    close(server_socket);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Чтение данных от клиента
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        log("Ошибка при чтении данных от клиента");
        close(client_socket);
        return;
    }

    // Преобразуем запрос в строку для анализа
    std::string request(buffer, bytes_received);
    log("Получен запрос от клиента:\n" + request);

    // Проверяем, является ли запрос CONNECT-запросом
    std::string host;
    int port;
    if (extract_host_and_port(request, host, port)) {
        log("Обработка CONNECT-запроса для: " + host + ":" + std::to_string(port));
        handle_connect_request(client_socket, host, port);
    } else {
        log("Неизвестный тип запроса");
        close(client_socket);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        log("Ошибка при создании сокета");
        return 1;
    }

    // Настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Порт прокси

    // Привязка сокета
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log("Ошибка при привязке сокета");
        close(server_socket);
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(server_socket, 10) < 0) {
        log("Ошибка при прослушивании");
        close(server_socket);
        return 1;
    }

    log("Прокси-сервер запущен на порту 8080");

    std::vector<std::thread> threads;

    while (true) {
        // Принятие входящего соединения
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            log("Ошибка при принятии соединения");
            continue;
        }

        // Обработка клиента в отдельном потоке
        threads.emplace_back(handle_client, client_socket);
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    close(server_socket);
    return 0;
}
