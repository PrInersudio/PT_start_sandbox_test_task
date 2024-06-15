#ifndef FILE_INFO
#define FILE_INFO

#include <stdlib.h>

// структрура для хранения считанного содержимого и размера файла
struct file_info {
    unsigned char* content;
    size_t size;
};

//чтение всего содержимого файла
int file_info(struct file_info* fi, const char* path);

// освобождение памяти, выделенной под храниение сожеджимого файла
void free_file_info(struct file_info* fi);

// проверка существования файла
int check_file_existance(int sock, char* path);


// проверяет существование директории, если её нет, создаёт её
int check_directory(char* path);

#endif