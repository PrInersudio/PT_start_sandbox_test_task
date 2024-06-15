#include "CheckLocalFile_server.h"
#include "file_info.h"
#include "sizet_list.h"
#include "config.h"
#include "sys/socket.h"
#include <string.h>
#include "multithread_tcp.h"
#include "handle_json.h"
#include "b64/b64.h"
#include "substring.h"

// определнние размера двоичных данных, закодированных в base64
size_t b64_decoded_size(const char *in) {
	size_t len;
	size_t ret;
	size_t i;

	if (in == NULL)
		return 0;

	len = strlen(in);
	ret = len / 4 * 3;

	for (i=len; i-->0; ) {
		if (in[i] == '=') {
			ret--;
		} else {
			break;
		}
	}

	return ret;
}

// считывание параметров CheckLocalFile
int get_CheckLocalFile_params (int sock, cJSON* json_params, struct CheckLocalFile* params) {
    log("Получение параметров для CheckLocalFile.\n");
    params->path = parse_json_string(sock, json_params, "path");
    char* b64sig = parse_json_string(sock, json_params, "sig");
    if ((params->path == NULL) || (b64sig == NULL)) return 2;
    log("Попытка декодирования сигнатуры.\n");
    if ((params->sig_len = b64_decoded_size(b64sig)) == 0) {
        log("Неправильный размер base64 сигнатуры. Номер сокета %d.\n", sock);
        const char b64_decoded_size_error_message[] = "Неправильный размер base64 сигнатуры.\n";
        send(sock, b64_decoded_size_error_message, strlen(b64_decoded_size_error_message), 0);
        return 3;
    }
    if ((params->sig = b64_decode(b64sig, strlen(b64sig))) == NULL) {
        log("Ошибка при декодировании сигнатуры. Номер сокета %d.\n", sock);
        const char b64_decode_error_message[] = "Ошибка при декодировании сигнатуры.\n";
        send(sock, b64_decode_error_message, strlen(b64_decode_error_message), 0);
        return 4;
    }
    log("Параметры CheckLocalFile получены.\n");
    return 0;
}

void free_CheckLocalFile(struct CheckLocalFile* params) {
    free(params->sig);
    params->sig = NULL;
    params->path = NULL; // её не надо освобождать здесь, она при удалении json занулится
}


// обработка запроса CheckLocalFile
void CheckLocalFile(int sock, cJSON* json_params) {
    // чтение параметров
    struct CheckLocalFile params;
    if (get_CheckLocalFile_params(sock, json_params, &params)) return;
    // чтение файла
    struct file_info fi;
    if (lock_mutex(sock)) {free_CheckLocalFile(&params); return;}
    if (!check_file_existance(sock, params.path)) {
        free_CheckLocalFile(&params); 
        unlock_mutex(sock); 
        return;
    }
    int read_file_result = file_info(&fi, params.path);
    unlock_mutex(sock);
    if (read_file_result) {
        const char read_all_file_error_message[] = "Ошибка при чтении файла.\n";
        send(sock, read_all_file_error_message, strlen(read_all_file_error_message), 0);
        free_CheckLocalFile(&params);
        return;
    }
    // получение смещений
    struct sizet_list offsets;
    if (init_sizet_list(&offsets)) {
        const char init_sizet_list_error_message[] = "Ошибка памяти на сервере.\n";
        send(sock, init_sizet_list_error_message, strlen(init_sizet_list_error_message), 0);
        free_file_info(&fi);
        free_CheckLocalFile(&params);
        return;
    }
    if (substring(&offsets, fi.content, fi.size, params.sig, params.sig_len)) {
        const char substring_error_message[] = "Ошибка памяти на сервере.\n";
        send(sock, substring_error_message, strlen(substring_error_message), 0);
        free_file_info(&fi);
        free_CheckLocalFile(&params);
        free_sizet_list(&offsets);
        return;
    }
    log("Формирование ответа CheckLocalFile. Номер сокета %d.\n", sock);
    char response[RESPONSE_SIZE] = {0};
    size_t response_len = 0;
    if (offsets.size != 0) {
        const char response_prefix[] = "Сигнатура найдена на смещениях: ";
        strncpy(response, response_prefix, strlen(response_prefix)+1);
        response_len = str_sizet_list(&offsets, response, strlen(response_prefix), RESPONSE_SIZE);
    }
    else {
        const char response_prefix[] = "Сигнатура не найдена.";
        response_len = strlen(response_prefix);
        strncpy(response, response_prefix, response_len);
    }
    log("Будет отправлен ответ %s. Номер сокета %d.\n", response, sock);
    response[response_len] = '\n';
    send(sock, response, response_len, 0);
    // завершение работы
    free_file_info(&fi);
    free_CheckLocalFile(&params);
    free_sizet_list(&offsets);
}