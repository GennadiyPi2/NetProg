#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Создаем UDP-сокет
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return -1;
    }

    // Настраиваем адрес сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Принимаем сообщения от всех интерфейсов
    server_addr.sin_port = htons(PORT); // Порт

    // Привязываем сокет к адресу и порту
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true) {
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *) &client_addr, &addr_len);
        buffer[n] = '0'; // Завершаем строку

        std::cout << "Получено сообщение от клиента (" 
                  << inet_ntoa(client_addr.sin_addr) << "): " 
                  << buffer << std::endl;

        // Отправляем ответ клиенту
        const char *response = "Сообщение принято!";
        sendto(sockfd, response, strlen(response), MSG_CONFIRM,
               (const struct sockaddr *) &client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}

