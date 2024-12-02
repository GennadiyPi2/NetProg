#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1" // Замените на IP-адрес сервера
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Создаем UDP-сокет
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return -1;
    }

    // Настраиваем адрес сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT); // Порт

    // Преобразуем IP-адрес сервера из строки в бинарный формат
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес или адрес не поддерживается" << std::endl;
        close(sockfd);
        return -1;
    }

    while (true) {
        std::string message;
        std::cout << "Введите сообщение для отправки (или 'exit' для выхода): ";
        std::getline(std::cin, message);

        // Выходим из цикла, если пользователь ввел 'exit'
        if (message == "exit") {
            break;
        }

        // Отправляем сообщение на сервер
        sendto(sockfd, message.c_str(), message.size(), MSG_CONFIRM,
               (const struct sockaddr *) &server_addr, sizeof(server_addr));

        // Получаем ответ от сервера
        socklen_t addr_len = sizeof(server_addr);
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *) &server_addr, &addr_len);
        buffer[n] = '0'; // Завершаем строку

        std::cout << "Ответ от сервера: " << buffer << std::endl;
    }
    
        close(sockfd);
    return 0;
}

