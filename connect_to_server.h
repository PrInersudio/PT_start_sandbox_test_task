#ifndef CONNECT_TO_SERVER
#define CONNECT_TO_SERVER

#include <netinet/in.h>

// соединение с сервером
int connect_to_server(int* sock,struct sockaddr_in* server_addr);

// получает сообщение от сервера и печатает его
int recv_and_print(int sock);

#endif