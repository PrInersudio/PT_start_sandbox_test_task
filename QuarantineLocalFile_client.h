#ifndef QUARANTINELOCALFILE_CLIENT
#define QUARANTINELOCALFILE_CLIENT

#include "cJSON/cJSON.h"

// добавление параметров в json для запроса QuarantineLocalFile
int QuarantineLocalFile(cJSON* json_params, const char* path);

#endif