#ifndef QUARANTINELOCALFILE_SERVER
#define QUARANTINELOCALFILE_SERVER

#include "cJSON/cJSON.h"

extern char* quarantine_path;

// обработка запроса QuarantineLocalFile
void QuarantineLocalFile(int sock, cJSON* params);

#endif