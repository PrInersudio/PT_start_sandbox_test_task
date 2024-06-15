#ifndef HANDLE_JSON
#define HANDLE_JSON

#include "cJSON/cJSON.h"

// чтение json
cJSON* parse_json(int sock, char* buf);

// чтение строки из json
char* parse_json_string(int sock, cJSON* json, const char* field_name);

// чтение объекта из json
cJSON* parse_json_obj(int sock, cJSON* json, const char* field_name);

#endif