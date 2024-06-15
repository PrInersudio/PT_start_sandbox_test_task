#include <netinet/in.h>
#include <pthread.h>
#include "config.h"
#include "multithread_tcp.h"
#include "set_server_address.h"
#include "QuarantineLocalFile_server.h"
#include "file_info.h"

int main(int argc, char** argv) {
    log("Логгирование включено.\n");
    if (argc < 5) {server_help(); return 0;}
    if (pthread_mutex_init(&mutex, NULL)) {
        perror("Инициализация мьютекса не удалась.\n");
        return 1;
    }
    // обработка параметров коммандной строки
    quarantine_path = argv[4];
    if (check_directory(quarantine_path)) return 3;
    // открытие подключения
    struct sockaddr_in server_addr;
    if (set_server_address(&server_addr, argv[1], argv[2])
        || open_connection(&server_socket, &server_addr, argv[3])) return 4;
    // Привязка обработчика sigint
    signal(SIGINT, handler);
    log("Обработчик SIGINT привязан.\n");
    // Принимаем подключения в цикле, пока не послан SIGINT
    while (!stop_server) {
        if ((client_connect(server_socket)) && (!stop_server)) continue;
        if (stop_server) break;
        open_client_thread(client_socket);
        client_socket = NULL;
    }
    return 0;
}