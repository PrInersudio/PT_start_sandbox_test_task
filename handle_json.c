#include "handle_json.h"
#include "config.h"
#include <sys/socket.h>
#include <string.h>

// чтение json
cJSON* parse_json(int sock, char* buf) {
    log("Чтение json из строки %s. Номер сокета %d.\n", buf, sock);
    cJSON* json = cJSON_Parse(buf);
    if (json != NULL) {
        log("Чтение json из строки %s успешно. Номер сокета %d.\n", buf, sock);
        return json;
    }
    // обработка ошибки чтения json
    const char* error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
        log("Ошибка чтения json: %s. Номер сокета %d.\n", error_ptr, sock);
    }
    else {
        log("Ошибка чтения json: NULL. Номер сокета %d.\n", sock);
    }
    const char failed_json_read_message[] = "Ошибка чтения json.\n";
    send(sock, failed_json_read_message, strlen(failed_json_read_message), 0);
    return NULL;
}

// чтение строки из json
char* parse_json_string(int sock, cJSON* json, const char* field_name) {
    log("Считывание строки из поля %s. Номер сокета %d\n.", field_name, sock);
    cJSON* string = cJSON_GetObjectItemCaseSensitive(json, field_name);
    if (!cJSON_IsString(string) || (string->valuestring == NULL)) {
        log("Ошибка при считывании строки из поля %s. Номер сокета %d\n.", field_name, sock);
        const char failed_string_parse_message[] = "Ошибка чтения строки из json.\n";
        send(sock, failed_string_parse_message, strlen(failed_string_parse_message), 0);
        return NULL;
    }
    log("Считана строка из json %s\n", string->valuestring);
    return string->valuestring;
}

// чтение объекта из json
cJSON* parse_json_obj(int sock, cJSON* json, const char* field_name) {
    log("Cчитывание json объкта из поля %s. Номер сокета %d.\n", field_name, sock);
    cJSON* obj = cJSON_GetObjectItemCaseSensitive(json, field_name);
    if (!cJSON_IsObject(obj)) {
        log("Ошибка при считывании json объкта из поля %s. Номер сокета %d\n.", field_name, sock);
        const char failed_obj_parse_message[] = "Ошибка чтения объекта из json.\n";
        send(sock, failed_obj_parse_message, strlen(failed_obj_parse_message), 0);
        return NULL;
    }
    log("Считан объект json.\n");
    return obj;
}