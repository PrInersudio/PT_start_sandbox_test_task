#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "set_server_address.h"
#include "json_request.h"
#include "connect_to_server.h"

int main(int argc, char** argv) {
    log("Логгирование включено.\n");
    // формирование запроса
    char request[REQUEST_SIZE];
    if (prepare_json_request(request, argc, argv)) {
        log("Неудача при формировании запроса.\n");
        perror("Неудача при формировании запроса.\n");
        return 1;
    }
    // соединение с сервером
    struct sockaddr_in server_addr;
    int sock = 0;
    if (set_server_address(&server_addr, argv[1], argv[2]) ||
        connect_to_server(&sock, &server_addr)) return 2;
    // общение с сервером
    if (
        (recv_and_print(sock)) || // получение приветственного сообщения от сервера
        (send(sock, request, strlen(request), 0) < 0) || // отправака запроса
        (recv_and_print(sock)) // получение результатов запроса
    ) {
        log("Ошибка при общении с сервером.\n");
        perror("Ошибка при общении с сервером.\n");
        return 3;
    }
    log("Общение с сервером завершено.\n");
    close(sock);
    return 0;
}