#include <sys/socket.h>
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "handle_json.h"
#include "CheckLocalFile_server.h"
#include "QuarantineLocalFile_server.h"
#include "string.h"

// приветственное сообщение
void send_welcome_message(int sock) {
    log("Отправка приветственного сообщения. Номер сокета %d.\n", sock);
    const char welcome_message[] = "Вы успешно подключены, ожидаю команду.\n";
    send(sock, welcome_message, strlen(welcome_message), 0);
    log("Приветственное сообщение отправлено. Номер сокета %d.\n", sock);
}

// приём сообщения от клиента
int recv_json(int sock, char* buf) {
    log("Приём сообщения от клиента. Номер сокета %d.\n", sock);
    ssize_t read_size = recv(sock, buf, RESPONSE_SIZE, 0);
    if (read_size <= 0) {
        log("Произошла ошибка при приёме сообщения. Номер сокета %d.\n", sock);
        const char failed_recv_message[] = "Произошла ошибка при приёме сообщения.\n";
        send(sock, failed_recv_message, strlen(failed_recv_message), 0);
        return read_size;
    }
    buf[read_size] = '\0';
    log("Получено сообщение %s. Номер сокета %d.\n", buf, sock);
    return read_size;
}

// выбор команды и её выполнение
void send_response(int sock, char* command_name, cJSON* params) {
    log("Выбор команды. Номер сокета %d.\n", sock);
    const char CheckLocalFile_string[] = "CheckLocalFile";
    const char QuarantineLocalFile_string[] = "QuarantineLocalFile";
    if (!strncmp(CheckLocalFile_string, command_name, strlen(CheckLocalFile_string))) {
        log("Выбрана команда CheckLocalFile. Номер сокета %d.\n", sock);
        CheckLocalFile(sock, params);
        return;
    }
    if (!strncmp(QuarantineLocalFile_string, command_name, strlen(QuarantineLocalFile_string))) {
        log("Выбрана команда QuarantineLocalFile. Номер сокета %d.\n", sock);
        QuarantineLocalFile(sock, params);
        return;
    }
    log("Неправильная команда. Номер сокета %d.\n", sock);
    const char incorrect_command_message[] = "Неправильная команда.\n";
    send(sock, incorrect_command_message, strlen(incorrect_command_message), 0);
}

// обработчик запроса клиента
void* handle_client(void* client_socket) {
    int sock = *(int*)client_socket;
    free(client_socket);
    client_socket = NULL;
    log("Обработка запроса. Номер сокета %d\n", sock);
    cJSON* json = NULL;
    send_welcome_message(sock);
    char buf[REQUEST_SIZE];
    if (recv_json(sock, buf) <= 0) goto handle_client_end;
    json = parse_json(sock, buf);
    if (json == NULL) goto handle_client_end;
    char* command_name = parse_json_string(sock,json, "command1");
    if (command_name == NULL) goto handle_client_end;
    cJSON* params = parse_json_obj(sock, json, "params");
    if (params == NULL) goto handle_client_end;
    send_response(sock, command_name, params);
    log("Ответ отправлен.\n");
handle_client_end:
    log("Завершение работы обработчика запроса. Номер сокета %d\n", sock);
    close(sock);
    if (json != NULL) cJSON_Delete(json);
    json = NULL;
}