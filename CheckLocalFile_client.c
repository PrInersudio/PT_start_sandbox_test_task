#include "CheckLocalFile_client.h"
#include "file_info.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include "b64/b64.h"

// получение сигнатуры в base64 из параметра
int get_b64sig_from_param(char** b64sig, const char* sig_param) {
    log("Сигнатура будет получена из парметра %s.", sig_param);
    if ((*b64sig = (char*)malloc(sizeof(char) * (strlen(sig_param)+1))) == NULL) {
        log("Неудача при выделении памяти под base64 сигнатуру.\n");
        return 1;
    }
    strncpy(*b64sig, sig_param, strlen(sig_param)+1);
    log("Из параметров получена сигнатура в base64 %s.\n", *b64sig);
    return 0;
}

// получение сигнатуры в base64 из файла
int get_b64sig_from_file(char** b64sig, const char* sig_file) {
    log("Сигнатура будет получена из файла %s.", sig_file);
    struct file_info fi;
    if (file_info(&fi, sig_file)) {
        log("Ошибка чтения файла.\n");
        return 1;
    }
    if ((*b64sig = b64_encode(fi.content, fi.size)) == NULL) {
        log("Не удалось закодировать сигнатуру в base64.\n");
        free_file_info(&fi);
        return 2;
    }
    log("Получена сигнатура %s из файла %s.\n", *b64sig, sig_file);
    free_file_info(&fi);
    return 0;
}

// получение сигнатуры в base64
int get_b64sig(char** b64sig, const char* sig_param_type, const char* sig_param) {
    log("Получение сигнатуры в base64. Тип параметра: %s, параметр: %s.", sig_param_type, sig_param);
    if (!strncmp(sig_param_type, "b64sig", strlen("b64sig"))) {
        return get_b64sig_from_param(b64sig, sig_param);
    }
    if (!strncmp(sig_param_type, "sig_file", strlen("sig_file"))) {
        return get_b64sig_from_file(b64sig, sig_param);
    }
    log("Неизвестный тип параметра сигнатуры.\n");
    client_help(); return 1;
}

// добавление параметров в json для запроса CheckLocalFile
int CheckLocalFile(cJSON* json_params, const char* path, const char* sig_param_type, const char* sig_param) {
    log(
        "Добавление параметров %s, %s, %s в json для запроса CheckLocalFile.\n",
        path, sig_param_type, sig_param
    );
    char* b64sig;
    if (get_b64sig(&b64sig, sig_param_type, sig_param)) return 1;
    if (
        (cJSON_AddStringToObject(json_params, "path", path) == NULL) ||
        (cJSON_AddStringToObject(json_params, "sig", b64sig) == NULL)
    ) {
        log(
            "Не удалось добавить параметры %s, %s, %u в json объект json_params (дочерний json_request при формировании запроса CheckLocalFile.\n",
            path, b64sig
        );
        free(b64sig);
        b64sig = NULL;
        return 2;
    }
    log(
        "Добавлены параметры %s, %s, %s в json для запроса CheckLocalFile.\n",
        path, sig_param_type, sig_param
    );
    free(b64sig);
    b64sig = NULL;
    return 0;
}