#include <stdlib.h>
#include "QuarantineLocalFile_server.h"
#include "config.h"
#include <string.h>
#include <sys/socket.h>
#include "multithread_tcp.h"
#include "handle_json.h"
#include "file_info.h"

char* quarantine_path = NULL;

// обработка запроса QuarantineLocalFile
void QuarantineLocalFile(int sock, cJSON* params) {
    log("Считывание пути к файлу в QuarantineLocalFile.\n");
    char* path = parse_json_string(sock, params, "path");
    if (path == NULL) return;
    size_t name_offset;
    for (name_offset = strlen(path); name_offset != 0; --name_offset)
        if (path[name_offset] == '/') break;
    if (path[name_offset] == '/') ++name_offset;
    char new_path[MAXPATH];
    strncpy(new_path, quarantine_path, MAXPATH);
    strncat(new_path, "/", 2);
    strncat(new_path, path + name_offset, MAXPATH - strlen(new_path));
    log("Файл %s будет перемещён по пути %s.\n", path, new_path);
    if (lock_mutex(sock)) return;
    if (!check_file_existance(sock, path)) {unlock_mutex(sock); return;}
    int rename_result = rename(path, new_path);
    unlock_mutex(sock);
    if (rename_result) {
        log("Перемещение файла %s по пути %s не удалось.\n", path, new_path);
        const char rename_error_message[] = "Перемещение не удалось.\n";
        send(sock, rename_error_message, strlen(rename_error_message), 0);
        return;
    }
    log("Перемещение файла %s по пути %s успешно.\n", path, new_path);
    const char rename_success_message[] = "Перемещение успешно.\n";
    send(sock, rename_success_message, strlen(rename_success_message), 0);
}