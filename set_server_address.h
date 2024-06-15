#ifndef SET_SERVER_ADDRESS
#define SET_SERVER_ADDRESS

// настройка адреса
int set_server_address(struct sockaddr_in* server_addr, const char* server_addr_string, const char* port_string);

#endif