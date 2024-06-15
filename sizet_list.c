#include "sizet_list.h"
#include "config.h"
#include <string.h>

/*
    Инициализация списка.
    Пока она не проведена, поведение списка считается неопределённым.
    Возвращает 0 в случае успеха.
    В случае неуспеха возвращает не 0.
*/ 
int init_sizet_list(struct sizet_list* list) {
    log("Вызван init_sizet_list.\n");
    list->head = (struct sizet_node*)malloc(sizeof(struct sizet_node));
    if (list->head == NULL) {
        log("Ошибка выделения памяти при инициализации списка\n");
        return 1;
    }
    list->head->value = 0;
    list->head->next = NULL;
    list->tail = list->head;
    list->size = 0;
    log("Список инициализирован.\n");
    return 0;
}

/*
    Добавление элемента в спиок
    Возвращает 0 в случае успеха.
    В случае неуспеха возвращает не 0.
*/
int push_back_sizet_list(struct sizet_list* list, size_t value) {
    log("Вызван push_back_sizet_list.\n");
    list->tail->next = (struct sizet_node*)malloc(sizeof(struct sizet_node));
    if (list->tail->next == NULL) {
        log("Ошибка выделения памяти при добавлении элемента в список\n");
        return 1;
    }
    list->tail->value = value;
    list->tail = list->tail->next;
    list->tail->next = NULL;
    list->tail->value = 0;
    ++list->size;
    log("Добавлено значение %u.\n", value);
    return 0;
}

/*
    Переводит элементы списка list в строку 
    и конкантинирует их с переданной строкой str,
    начиная с позиции current_size.
    Работает пока не записаны все элементы списка
    и не превышен max_size.
    Возвращает размер новой строки (позицию \0).
*/
size_t str_sizet_list(struct sizet_list* list, char* str, size_t current_size, size_t max_size) {
    log("Вызван str_sizet_list.\n");
    struct sizet_node* current_node = list->head;
    while(current_node->next != NULL) {
        char current_value_string[MAX_SIZET_DECIMAL_STRING_SIZE] = {0};
        snprintf(current_value_string,
            MAX_SIZET_DECIMAL_STRING_SIZE,
            "%d ", current_node->value);
        log("Текущее число в size_t %u, в строке %s.\n", current_node->value, current_value_string);
        size_t new_size = current_size + strlen(current_value_string);
        if (new_size > max_size) break;
        strncat(str, current_value_string, strlen(current_value_string));
        current_size = new_size;
        current_node = current_node->next;
    }
    str[current_size-1] = '\0';
    log("Получена строка: %s.\n", str);
    return current_size;
}

// освобождение памяти, выделенную на список
void free_sizet_list(struct sizet_list* list) {
    log("Вызван free_sizet_list.\n");
    struct sizet_node* previous_node = NULL;
    struct sizet_node* current_node = list->head;
    while (current_node->next != NULL) {
        previous_node = current_node;
        current_node = current_node->next;
        previous_node->next = NULL;
        free(previous_node);
    }
    free(current_node);
    list->head = NULL;
    list->tail = NULL;
    log("Завершён free_sizet_list.\n");
}