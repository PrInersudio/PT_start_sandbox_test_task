#ifndef CHECKLOCALFILE_SERVER
#define HECKLOCALFILE_SERVER

#include<stdlib.h>
#include"cJSON/cJSON.h"

// структура для хранения параметров CheckLocalFile
struct CheckLocalFile {
    size_t sig_len;
    char* path;
    unsigned char* sig;
};

void CheckLocalFile(int sock, cJSON* json_params);

#endif