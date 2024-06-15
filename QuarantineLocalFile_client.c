#include "QuarantineLocalFile_client.h"
#include "config.h"

// добавление параметров в json для запроса QuarantineLocalFile
int QuarantineLocalFile(cJSON* json_params, const char* path) {
    log("Добавление параметров %s в json для запроса QuarantineLocalFile.\n", path);
    if (cJSON_AddStringToObject(json_params, "path", path) == NULL) {
        log(
            "Не удалось добавить параметры %s в json объект json_params (дочерний json_request) при формировании запроса QuarantineLocalFile.\n",
            path
        );
        return 1;
    }
    log("Добавлен параметр %s в json для запроса QuarantineLocalFile.\n", path);
    return 0;
}