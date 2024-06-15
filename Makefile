# вывести справку
all:
	@echo "make - вывести эту справку"
	@echo "make logging - создаёт сервер и клиент с включённым логгированием."
	@echo "make nologging - создаёт сервер и клиент с выключенным логгированием."
	@echo "make server_logging - создаёт сервер с включённым логгированием."
	@echo "make client_logging - создаёт клиент с включённым логгированием."
	@echo "make server_nologging - создаёт сервер с выключенным логгированием."
	@echo "make client_nologging - создаёт клиент с выключенным логгированием."
	@echo "make server_help - вывод инструкции о запуске сервера."
	@echo "make client_help - вывод инструкции о запуске клиента."
	@echo "make remove - очистка директории от объектных файлов, исполняемых файлов, тестовых директорий."
	@echo "make remove_objects - очистка директории от объектных файлов."
	@echo "make remove_exes - очистка директории от исполняемых файлов."
	@echo "make remove_test_dirs - очистка директории от тестовых директорий."
	@echo "\033[31mВнимание! Если вы уже собрали приложение с включённым логгированием и хотите \
	собрать версию без логгирования или наоборот, предварительно очистите директорию от объектных \
	файлов (командой make remove, командой make remove_objects или как-либо ещё).\033[0m"


# создаёт сервер и клиент с включённым логгированием
logging: server_logging client_logging

# создаёт сервер и клиент с выключенным логгированием
nologging: server_nologging client_nologging

# создаёт сервер с включённым логгированием
server_logging:
	$(MAKE) -f Logging.mk server

# создаёт клиент с включённым логгированием
client_logging:
	$(MAKE) -f Logging.mk client

# создаёт сервер с выключенным логгированием
server_nologging:
	$(MAKE) -f Nologging.mk server

# создаёт клиент с выключенным логгированием
client_nologging:
	$(MAKE) -f Nologging.mk client

include common.mk

# вывод инструкции о запуске сервера
server_help:
	@echo "Запускать сервер следующим образом: $(SERVER_APP_FILE) <ip сервера> <порт> <количество потоков> <директория карантина>"

# вывод инструкции о запуске клиента
client_help:
	@echo "Запускать клиент следующим образом:"
	@echo "$(CLIENT_APP_FILE) <ip сервера> <порт> CheckLocalFile <путь на сервере> b64sig <сигнатура в base64>"
	@echo "$(CLIENT_APP_FILE) <ip сервера> <порт> CheckLocalFile <путь на сервере> sig_file <файл с сигнатурой>"
	@echo "$(CLIENT_APP_FILE) <ip сервера> <порт> QuarantineLocalFile <путь на сервере>"

# очистка директории от объектных файлов, исполняемых файлов, тестовых директорий
remove: remove_exes remove_objects remove_test_dirs

# очистка директории от объектных файлов
remove_objects:
	rm -f *.o

# очистка директории от исполняемых файлов
remove_exes:
	rm -f *.exe

# очистка директории от тестовых директорий
remove_test_dirs:
	rm -rf тестовые_файлы карантин
