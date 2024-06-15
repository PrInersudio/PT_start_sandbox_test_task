import subprocess
import threading
import os
import random
from base64 import b64encode

IP = "127.0.0.1"
PORT = "2555"
TEST_FILES_DIR = "тестовые_файлы"
MAX_SIG_SIZE = 1024
CLIENT_APP = "./client.exe"

# генерация файла с произвольным повторением сигнатур
def generate_test_file(path: str, sig: bytes) -> None:
    file_size = random.randint(2 * len(sig), 1 << 20)  # Размер файла от двойного размера сигнатуры до 1 МБ
    # выбор промежутков между позициями сигнатур
    remaining_size = file_size
    gap_lengths = []
    while remaining_size > 0:
        if remaining_size <= len(sig): break
        gap_length = random.randint(0, remaining_size - len(sig))
        gap_lengths.append(gap_length)
        remaining_size -= gap_length + len(sig)
    # генерация файла
    sig_positions = []
    with open(path, 'wb') as f:
        current_pos = 0
        for gap_length in gap_lengths:
            f.write(os.urandom(gap_length))
            current_pos += gap_length
            sig_positions.append(current_pos)
            f.write(sig)
            current_pos += len(sig)
        if current_pos < file_size:
            f.write(os.urandom(file_size - current_pos))
        print("Файл:", path, "Позиции сигнатур:", sig_positions)


# генерация набора
def generate_test_files(num_files: int) -> list:
    os.makedirs(TEST_FILES_DIR, exist_ok=True)
    paths = []
    for i in range(num_files):
        sig_size = random.randint(1, MAX_SIG_SIZE)
        sig = os.urandom(sig_size)
        path = os.path.join(TEST_FILES_DIR, f"test_file_{i}.bin")
        generate_test_file(path, sig)
        paths.append((path, b64encode(sig).decode()))
    return paths

output_list = []
output_lock = threading.Lock()

# Функция для запуска клиентского приложения с заданными параметрами
def run_client(params):
    output = ""
    try:
        result = subprocess.run(params, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output = "Команда: " + ' '.join(params) + \
                    "\n\tВывод:\n" + \
                        "\t\t".join(result.stdout.splitlines(True)) + \
                    "\n\tОшибки:\n" + \
                        "\t\t".join(result.stderr.splitlines(True))
    except Exception as e:
        output = "Ошибка запуска подпроцесса:" + repr(e) + ". Команда: " + ' '.join(params)
    with output_lock:
        output_list.append(output)

def main():
    paths_and_sigs = generate_test_files(8)

    clients_params = [
        [CLIENT_APP, IP, PORT, "CheckLocalFile", paths_and_sigs[0][0], "b64sig", paths_and_sigs[0][1]],
        [CLIENT_APP, IP, PORT, "CheckLocalFile", paths_and_sigs[0][0], "b64sig", paths_and_sigs[0][1]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[0][0]],
        [CLIENT_APP, IP, PORT, "CheckLocalFile", paths_and_sigs[0][0], "b64sig", paths_and_sigs[0][1]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[0][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[1][0]],
        [CLIENT_APP, IP, PORT, "CheckLocalFile", paths_and_sigs[1][0], "b64sig", paths_and_sigs[1][1]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[1][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[2][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[3][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[4][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[4][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[5][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[6][0]],
        [CLIENT_APP, IP, PORT, "QuarantineLocalFile", paths_and_sigs[7][0]],
    ]

    threads = []
    
    for params in clients_params:
        thread = threading.Thread(target=run_client, args=(params,))
        threads.append(thread)
        thread.start()
    
    for thread in threads:
        thread.join()
    
    for output in output_list:
        print(output)

if __name__ == "__main__":
    main()
