#ifndef MULTITHREAD_TCP
#define MULTITHREAD_TCP

#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

extern pthread_mutex_t mutex;

// volatile - не оптимизировать компилятором,
// sig_atomic_t - не прерывать обработку другими операциями 
extern volatile sig_atomic_t stop_server;

extern int server_socket;
extern int* client_socket;

// открытие подключения
int open_connection(int* server_socket, struct sockaddr_in* server_addr, char* str_threads_quantity);

// принять подключение от клиета
int client_connect(int server_socket);

// создание потока для обработки запросов клиента
void open_client_thread(int* client_socket);

// проверка корректности ввода порта в параметрах коммандной строки
int port_to_int(char* str_port, uint16_t* port);

// проверка корректности ввода количества потоков в параметрах коммандной строки
int threads_quantity_to_int(char* str_threads_quantity, int* threads_quantity);

// пытается закрыть мьютекс и логгирует результат
int lock_mutex(int sock);

// пытается открыть мьютекс и логгирует результат
void unlock_mutex(int sock);

// обработчик SIGINT
void handler(int signal);

#endif