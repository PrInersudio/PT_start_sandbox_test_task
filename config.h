#ifndef CONFIG
#define CONFIG
#include <stdio.h>

#ifdef LOGGING
    #define log(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define log(fmt, ...)
#endif
// максимальная длина пути к файлу
#define MAXPATH 4096
// 1368 - размер последовательности размером 1 Кб, закодированной в base64
// 255 - запас на на элеметы конструкции json
#define REQUEST_SIZE (MAXPATH + 1368 + 255)
#define RESPONSE_SIZE 4096
// size_t обычно беззнаковое 64 бита
// максимальное значение 2^64-1 = 18,446,744,073,709,551,615
// 20 цифр
// ещё надо учесть \0
#define MAX_SIZET_DECIMAL_STRING_SIZE 21
// 255 - запас
#define CHECK_DIRECTORY_COMMAND_SIZE (MAXPATH + MAXPATH + 255)

// выводит сообщение с помощью в правильном запуске клиента
#define client_help() \
    log("Вызван help.\n"); \
    printf("Запускать следующим образом:\n"); \
    printf("<файл с приложением> <ip сервера> <порт> CheckLocalFile <путь на сервере> b64sig <сигнатура в base64>\n"); \
    printf("<файл с приложением> <ip сервера> <порт> CheckLocalFile <путь на сервере> sig_file <файл с сигнатурой>\n"); \
    printf("<файл с приложением> <ip сервера> <порт> QuarantineLocalFile <путь на сервере>\n");

// выводит сообщение с помощью в правильном запуске сервера
#define server_help() \
    log("Неправильное количество параметров командной строки. Должно быть 5.\n"); \
    printf("Запускать следующим образом: <файл с приложением> <ip сервера> <порт> <количество потоков> <директория карантина>\n");

#endif