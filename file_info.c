#include "config.h"
#include "file_info.h"
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>

//чтение всего содержимого файла
int file_info(struct file_info* fi, const char* path) {
    log("Открытие файла %s.\n", path);
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        log("Ошибка при открытии файла %s.\n", path);
        return 1;
    }
    log("Открыт файл %s.\n", path);
    log("Получение размера файла %s.\n", path);
    fseek(fp, 0, SEEK_END);
    fi->size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    log("Получен размер файла %s.\n", path);
    log("Выделение памяти под хранение содержимого файла %s.\n", path);
    fi->content = (unsigned char*)malloc(sizeof(unsigned char) * fi->size);
    if (fi->content == NULL) {
        log("Ошибка при выделении память под хранение содержимого файла %s.\n", path);
        return 2;
    }
    log("Выделена память под хранение содержимого файла %s.\n", path);
    log("Считывание файла %s и его закрытие.\n", path);
    fread(fi->content, 1, fi->size, fp);
    fclose(fp);
    fp = NULL;
    log("Файл %s прочитан.\n", path);
    return 0;
}

// освобождение памяти, выделенной под храниение содержимого файла
void free_file_info(struct file_info* fi) {
    log("Освобождение памяти, выделенной под храниение содержимого файла.\n");
    free(fi->content);
    fi->content = NULL;
    log("Освобождена память, выделенная под храниение содержимого файла.\n");
}

// проверка существования файла
int check_file_existance(int sock, char* path) {
    log("Проверка существования файла %s. Номер сокета %d.\n", path, sock);
    struct stat s;
    if (stat(path, &s) || (!S_ISREG(s.st_mode))) {
        log("%s не существует или не является файлом. Номер сокета %d.\n", path, sock);
        const char stat_error_message[] = "Не существует или не является файлом.\n";
        send(sock, stat_error_message, strlen(stat_error_message), 0);
        return 0;
    }
    log("%s существует и является файлом. Номер сокета %d.\n", path, sock);
    return 1;
}

// проверяет существование директории, если её нет, создаёт её
int check_directory(char* path) {
    // if [ ! -d path ]; then mkdir -p path fi
    char command[CHECK_DIRECTORY_COMMAND_SIZE];
    snprintf(command, CHECK_DIRECTORY_COMMAND_SIZE,
        "if [ ! -d %s ]; then mkdir -p %s; fi", path, path);
    log("Будет выполена команада shell: %s.\n", command);
    if (system(command)) {
        log("Ошибка при проверке существования директории %s", path);
        return 1;
    }
    log("Директрория %s существует или создана", path);
    return 0;
}