#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "172.16.40.1"
#define SERVER_PORT 7
#define BUFFER_SIZE 1024

int main() {
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    // Установка адреса сервера
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Ошибка при установке адреса сервера" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Подключение к серверу
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Ошибка при подключении к серверу" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Отправка сообщения на сервер
    const char* message = "Hello, server!";
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Ошибка при отправке сообщения на сервер" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Получение ответа от сервера
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
        std::cerr << "Ошибка при получении ответа от сервера" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Вывод ответа от сервера
    std::cout << "Ответ от сервера: " << buffer << std::endl;

    // Закрытие сокета
    close(clientSocket);

    return 0;
}
