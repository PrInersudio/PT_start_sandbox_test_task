#include <pthread.h>
#include "multithread_tcp.h"
#include <string.h>
#include "config.h"
#include "handle_client.h"
#include <unistd.h>

pthread_mutex_t mutex;

// volatile - не оптимизировать компилятором,
// sig_atomic_t - не прерывать обработку другими операциями 
volatile sig_atomic_t stop_server = 0;

int server_socket = -1;
int* client_socket = NULL;

// пытается закрыть мьютекс и логгирует результат
int lock_mutex(int sock) {
    log("Закрытие мьютекса. Номер сокета %d.\n", sock);
    if (pthread_mutex_lock(&mutex)) {
        log("Не удалось закрыть мьютекс. Номер сокета %d.\n", sock);
        const char mutex_lock_error_message[] = "Внутренняя ошибка на сервере.\n";
        send(sock, mutex_lock_error_message, strlen(mutex_lock_error_message), 0);
        return 1;
    }
    log("Мьютекс закрыт. Номер сокета %d.\n", sock);
    return 0;
}

// пытается открыть мьютекс и логгирует результат
void unlock_mutex(int sock) {
    log("Открытие мьютекса. Номер сокета %d.\n", sock);
    if (pthread_mutex_unlock(&mutex)) log("Не удалось открыть мьютекс. Номер сокета %d.\n", sock);
    log("Мьютекс открыт. Номер сокета %d.\n", sock);
}

// обработчик SIGINT
void handler(int signal) {
    log("Получен сигнал SIGINT. Закрытие сокета и завершение работы сервера.\n");
    stop_server = 1;
    if (client_socket != NULL) {
        free(client_socket);
        client_socket = NULL;
    }
    if (server_socket >= 0) close(server_socket);
    pthread_mutex_destroy(&mutex);
    log("До свидания.\n");
    exit(0);
}

// открытие подключения
int open_connection(int* server_socket, struct sockaddr_in* server_addr, char* str_threads_quantity) {
    log("Перевод количества потоков в число.\n");
    int threads_quantity = atoi(str_threads_quantity);
    if (threads_quantity <= 0) {
        log("Некорректное количество потоковю.\n");
        perror("Количество потоков должно быть больше 0.\n");
        return 1;
    }
    log("Количество потоков переведено в число.\n");
    log("Создание ipv4 tcp сокета.\n");
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_socket == -1) {
        perror("Не удалось открыть сокет.\n");
        return 2;
    }
    log("Сокет создан.\n");
    log("Привязка сокета к порту.\n");
    if (bind(
        *server_socket,
        (struct sockaddr*)server_addr,
        sizeof(struct sockaddr_in)
        ) < 0) {
        perror("Привязка сокета к порту не удалась.\n");
        close(*server_socket);
        return 3;
    }
    log("Адрес привязан.\n");
    log("Прослушивание входящих подключений.\n");
    if ((listen(*server_socket, threads_quantity)) != 0) { 
        printf("Прослушивание входящих подключений не удалось.\n"); 
        return 4; 
    } 
    log("Ждём подключение к серверу.\n");
    return 0;
}

// принять подключение от клиета
int client_connect(int server_socket) {
    log("Принимаем подключение от клиета.\n");
    client_socket = (int*)malloc(sizeof(int));
    if (client_socket == NULL) {
        log("Ошибка при выделении памяти под клиентский сокет. Сервер продолжает ждать другие подключения.\n");
        return 1;
    }
    log("Выделена память под номер сокета.\n");
    log("Принятие подключения.\n");
    socklen_t client_addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    if ((*client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
        free(client_socket);
        client_socket = NULL;
        log("Произошла ошибка подключения. Сервер продолжает ждать другие подключения.\n");
        return 2;
    }
    log("Подключение принято.\n");
    log("Проверяем, не выключен ли сервер.\n");
    if (stop_server) {
        close(*client_socket);
        free(client_socket);
        client_socket = NULL;
        log("Сервер остановлен.\n");
        return 3;
    }
    log("Не выключен.\n");
    log("Подключение принято от %s. Номер сокета %d.\n", inet_ntoa(client_addr.sin_addr), *client_socket);
    return 0;
}

// создание потока для обработки запросов клиента
void open_client_thread(int* client_socket) {
    log("Создание потока для обработки запросов клиента. Номер сокета %d.\n", *client_socket);
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client, (void *)client_socket) < 0) {
        log("Ошибка при создании потока. Сервер продолжает ждать другие подключения.\n");
        const char pthread_create_error_message[] = "Внутренняя ошибка сервера.\n";
        send(*client_socket, pthread_create_error_message, strlen(pthread_create_error_message), 0);
        close(*client_socket);
        free(client_socket);
        client_socket = NULL;
        return;
    }
    log("Поток для номера сокекта %d создан.\n", *client_socket);
    pthread_detach(thread_id);
}