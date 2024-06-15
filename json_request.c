#include "json_request.h"
#include "config.h"
#include <string.h>
#include "CheckLocalFile_client.h"
#include "QuarantineLocalFile_client.h"
#include <stdlib.h>

// создание json объекта с именем команды и получение объекта парметров
cJSON* create_json_request(cJSON** json_request, const char* command_name) {
    log("Cоздание json объекта %s с именем команды и получение объекта парметров.\n", command_name);
    log("Создание json объекта.\n");
    *json_request = cJSON_CreateObject();
    if (*json_request == NULL) {
        log("Не удалось создать json объект json_request при формировании запроса %s.\n", command_name);
        return NULL;
    }
    log("Запись имени команды и объекта параметров.\n");
    cJSON* json_params = NULL;
    if (
        (cJSON_AddStringToObject(*json_request, "command1", command_name) == NULL) ||
        ((json_params = cJSON_AddObjectToObject(*json_request, "params")) == NULL)
    ) {
        log("Не удалось добавить параметры в json объект json_request при формировании запроса %s.\n", command_name);
        cJSON_Delete(*json_request);
        *json_request = NULL;
        return NULL;
    }
    log("Создан объест json запроса %s.\n");
    return json_params;
}

// перевод json в строку и сохранение его в переменной для запроса
int json_to_string(char* request, cJSON* json_request) {
    log("Перевод json в строку и сохранение его в переменной для запроса.\n");
    char* json_string = cJSON_Print(json_request);
    if (json_string == NULL) {
        log("Не удалось преобразовать json в строку.\n");
        return 1;
    }
    log("Сформирован json запрос: %s.\n", json_string);
    strncpy(request, json_string, REQUEST_SIZE);
    free(json_string);
    return 0;
}

// формирование json для отправки на сервер
int prepare_json_request(char* request, int argc, char** argv) {
    log("Формирование json для отправки на сервер.\n");
    if (argc < 4) {client_help(); return 1;}
    cJSON* json_request = NULL;
    cJSON* json_params = create_json_request(&json_request, argv[3]);
    if (json_params == NULL) return 2;
    if (!strncmp(argv[3], "CheckLocalFile", strlen("CheckLocalFile"))) {
        log("Выбрана команда CheckLocalFile.\n");
        if (argc < 7) {client_help(); goto prepare_json_request_failure;}
        if (CheckLocalFile(json_params, argv[4], argv[5], argv[6])) goto prepare_json_request_failure;
    }
    else if (!strncmp(argv[3], "QuarantineLocalFile", strlen("QuarantineLocalFile"))) {
        log("Выбрана команда QuarantineLocalFile.\n");
        if (argc < 5) {client_help(); goto prepare_json_request_failure;}
        if (QuarantineLocalFile(json_params, argv[4])) goto prepare_json_request_failure;
    }
    else {
        log("Неизвестная команда.\n");
        client_help();
        goto prepare_json_request_failure;
    }
    if (json_to_string(request, json_request)) goto prepare_json_request_failure;
    cJSON_Delete(json_request);
    json_request = NULL; json_params = NULL;
    return 0;
prepare_json_request_failure:
    cJSON_Delete(json_request);
    json_request = NULL; json_params = NULL;
    return 3;
}