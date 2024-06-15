# команды
MAKE = make
CC = gcc

# Выходные файлы
SERVER_APP_FILE = ./server.exe
CLIENT_APP_FILE = ./client.exe

# Зависимости
SERVER_OBJS = server.o decode.o encode.o buffer.o cJSON.o sizet_list.o substring.o \
              file_info.o handle_client.o handle_json.o set_server_address.o\
              multithread_tcp.o CheckLocalFile_server.o QuarantineLocalFile_server.o

CLIENT_OBJS = client.o decode.o encode.o buffer.o cJSON.o \
              connect_to_server.o file_info.o set_server_address.o\
              json_request.o CheckLocalFile_client.o QuarantineLocalFile_client.o

# Правила для создания объектных файлов для b64 и cJSON
decode.o: b64/decode.c b64/b64.h config.h
	$(CC) -c -o $@ $<

encode.o: b64/encode.c b64/b64.h config.h
	$(CC) -c -o $@ $<

buffer.o: b64/buffer.c b64/b64.h config.h
	$(CC) -c -o $@ $<

cJSON.o: cJSON/cJSON.c cJSON/cJSON.h config.h
	$(CC) -c -o $@ $<