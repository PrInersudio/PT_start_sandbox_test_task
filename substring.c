#include "substring.h"
#include "config.h"

// равна длине максимального суффикса равного префиксу в подстроке [0, m]
size_t* prefix_function(const char* string, size_t string_len) {
    log("Вызван prefix_function(.\n");
    size_t* prefix_func = (size_t*)malloc(sizeof(size_t) * string_len);
    if (prefix_func == NULL) {
        log("Ошибка выделения памяти при подсчёте префикс-функции.\n");
        return NULL;
    }
    size_t k = prefix_func[0] = 0;
    for (size_t i = 1; i < string_len; ++i) {
        while((k > 0) && (string[k] != string[i])) --k;
        if (string[k] == string[i]) ++k;
        prefix_func[i] = k;
    }
    log("Выполнен prefix_function(.\n");
    return prefix_func;
}

/*
    Реализция поиска подстроки алгоритмом Кнута-Морриса-Пратта.
    Объединяем образец и строку, считаем для общей строки префиксную функцию.
    Если значение совпало с длиной образца, добавляем в ответ.
*/
int substring(struct sizet_list* offsets,
                const unsigned char* string, size_t string_len,
                const unsigned char* example, size_t example_len) {
    log("Вызван substring.\n");
    size_t* prefix_func = prefix_function(example, example_len);
    if (prefix_func == NULL) return 1;
    size_t k = 0;
    for (size_t i = 0; i < string_len; ++i) {
        while((k > 0) && (string[i] != example[k])) k = prefix_func[k-1];
        if (string[i] == example[k]) ++k;
        if (k == example_len) {
            push_back_sizet_list(offsets, i - example_len + 1);
            k = prefix_func[k-1];
        }
    }
    free(prefix_func);
    prefix_func = NULL;
    log("Выполнен substring.\n");
    return 0;
}