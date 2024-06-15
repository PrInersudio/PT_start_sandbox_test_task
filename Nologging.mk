include common.mk

# создаёт сервер с выключенным логгированием
server: $(SERVER_OBJS)
	$(CC) -o $(SERVER_APP_FILE) $^

# создаёт клиент с выключенным логгированием
client: $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_APP_FILE) $^

# Правила для создания объектных файлов без логгирования
%.o: %.c %.h config.h
	$(CC) -c -o $@ $<