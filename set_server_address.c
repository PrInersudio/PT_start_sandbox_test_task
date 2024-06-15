#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.h"
#include <stdlib.h>

// настройка адреса
int set_server_address(struct sockaddr_in* server_addr, const char* server_addr_string, const char* port_string) {
    log("Перевод порта в числовое значение.\n");
    server_addr->sin_family = AF_INET; // ipv4
    int port = atoi(port_string);
    if ((port <= 1000) || (port > 65535)) {
        log("Неправильный порт.\n");
        perror("Порт должен быть больше 1000 и меньше 65536.\n");
        server_help();
        return 1;
    }
    server_addr->sin_port = htons((uint16_t)port);
    log("Выставлен порт %u.\n", port);
    log("Настройка ip.\n");
    if (inet_pton(AF_INET, server_addr_string, &(server_addr->sin_addr)) <= 0) {
        log("Некорректный адрес.\n");
        perror("Некорректный адрес.\n");
        server_help();
        return 2;
    }
    log("Адрес настроен.\n");
    return 0;
}