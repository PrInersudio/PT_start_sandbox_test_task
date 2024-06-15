include common.mk

# define для включения логгирования
LOGGING_FLAG = -DLOGGING

# создаёт сервер с включённым логгированием
server: $(SERVER_OBJS)
	$(CC) $(LOGGING_FLAG ) -o $(SERVER_APP_FILE) $^

# создаёт клиент с включённым логгированием
client: $(CLIENT_OBJS)
	$(CC) $(LOGGING_FLAG) -o $(CLIENT_APP_FILE) $^

# Правила для создания объектных файлов с логгированием
%.o: %.c %.h config.h
	$(CC) $(LOGGING_FLAG) -c -o $@ $<