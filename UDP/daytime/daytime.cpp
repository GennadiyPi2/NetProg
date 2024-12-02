#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in serverAddr;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(13); // Порт службы daytime
    serverAddr.sin_addr.s_addr = inet_addr("172.16.40.1"); // IP-адрес сервера

    // Отправка запроса
    if (sendto(sockfd, nullptr, 0, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка при отправке запроса" << std::endl;
        return 1;
    }

    // Получение ответа
    ssize_t numBytes = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE - 1, 0, nullptr, nullptr);
    if (numBytes < 0) {
        std::cerr << "Ошибка при получении ответа" << std::endl;
        return 1;
    }

    // Добавление завершающего нулевого символа
    buffer[numBytes] = '\0';

    // Вывод ответа на экран
    std::cout << "Ответ от сервера: " << buffer << std::endl;

    // Закрытие сокета
    close(sockfd);

    return 0;
}
