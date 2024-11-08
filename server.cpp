#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Начало прослушивания
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true) {
        // Принятие входящего соединения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                 (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // Чтение запроса
        read(new_socket, buffer, 1024);
        std::cout << "Получен запрос:n" << buffer << std::endl;

        // Отправка ответа
        const char *response = "HTTP/1.1 200 OKrnContent-Type: text/plainrnrnHello, World!";
        send(new_socket, response, strlen(response), 0);
        std::cout << "Ответ отправленn" << std::endl;

        close(new_socket);
    }

    return 0;
}
