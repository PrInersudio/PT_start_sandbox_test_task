// реализация однонаправленного связанного списка
#ifndef SIZET_LIST
#define SIZET_LIST

#include<stdlib.h>

struct sizet_node {
    size_t value;
    struct sizet_node* next;
};

struct sizet_list {
    struct sizet_node* head;
    struct sizet_node* tail;
    size_t size;
};

/*
    Инициализация списка.
    Пока она не проведена, поведение списка считается неопределённым.
    Возвращает 0 в случае успеха.
    В случае неуспеха возвращает не 0.
*/ 
int init_sizet_list(struct sizet_list* list);

/*
    Добавление элемента в спиок
    Возвращает 0 в случае успеха.
    В случае неуспеха возвращает не 0.
*/
int push_back_sizet_list(struct sizet_list* list, size_t value);

/*
    Переводит элементы списка list в строку 
    и конкантинирует их с переданной строкой str,
    начиная с позиции current_size.
    Работает пока не записаны все элементы списка
    и не превышен max_size.
    Возвращает размер новой строки (позицию \0).
*/
size_t str_sizet_list(struct sizet_list* list, char* str, size_t current_size, size_t max_size);

// освобождение памяти, выделенную на список
void free_sizet_list(struct sizet_list* list);

#endif