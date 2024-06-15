#ifndef CHECKLOCALFILE_CLIENT
#define HECKLOCALFILE_CLIENT

#include "cJSON/cJSON.h"

// добавление параметров в json для запроса CheckLocalFile
int CheckLocalFile(cJSON* json_params, const char* path, const char* sig_param_type, const char* sig_param);

#endif