#include "connect_to_server.h"
#include "config.h"
#include <sys/socket.h>
#include <unistd.h>

// соединение с сервером
int connect_to_server(int* sock,struct sockaddr_in* server_addr) {
    log("Соединение с сервером.\n");
    log("Создание сокета.\n");
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log("Ошибка создания сокета.\n");
        perror("Ошибка создания сокета.\n");
        return 1;
    }
    log("Сокет создан.\n");
    log("Подключение к серверу.\n");
    if (connect(*sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0) {
        log("Ошибка подключения.\n");
        perror("Ошибка подключения.\n");
        close(*sock);
        return 2;
    }
    log("Соедиение установлено.\n");
    return 0;
}

// получает сообщение от сервера и печатает его
int recv_and_print(int sock) {
    log("Получение сообщение от сервера.\n");
    char buf[RESPONSE_SIZE];
    int bytes_received = recv(sock, buf, RESPONSE_SIZE - 1, 0);
    if (bytes_received < 0) {
        log("Ошибка получения данных.\n");
        perror("Ошибка получения данных.\n");
        return 1;
    }
    buf[bytes_received] = '\0';
    log("Получено от сервера: %s\n", buf);
    printf("Получено от сервера: %s\n", buf);
    return 0;
}