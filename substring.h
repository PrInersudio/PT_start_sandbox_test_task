#ifndef SUBSTRING
#define SUBSTRING
#include<stdlib.h>
#include "sizet_list.h"

/*
    Реализция поиска подстроки алгоритмом Кнута-Морриса-Пратта.
    Объединяем образец и строку, считаем для общей строки префиксную функцию.
    Если значение совпало с длиной образца, добавляем в ответ.
*/
int substring(struct sizet_list* offsets,
                const unsigned char* string, size_t string_len,
                const unsigned char* example, size_t example_len);

#endif