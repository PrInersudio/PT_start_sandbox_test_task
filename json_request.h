#ifndef JSON_REQUEST
#define JSON_REQUEST

#include "cJSON/cJSON.h"

// формирование json для отправки на сервер
int prepare_json_request(char* request, int argc, char** argv);

#endif