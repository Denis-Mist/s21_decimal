#include <stdio.h>
#include <stdlib.h>
#include"s21_types.h"
#include <limits.h>
#include<math.h>

#define OK 0
#define ERROR 1
#define MAX_DECIMAL_PLACES 7

void s21_clear_decimal(s21_decimal *decimal) {
    decimal->bits[0] = 0;
    decimal->bits[1] = 0;
    decimal->bits[2] = 0;
    decimal->bits[3] = 0;
}

void s21_set_sign(s21_decimal *decimal, int sign) {
    if (sign == 1) {
        decimal->bits[3] |= (1 << 31);
    }
}

void s21_print_decimal(s21_decimal decimal) {
    // for (int i = 3; i >= 0; i--) {
    //     for (int j = 31; j >= 0; j--) {
    //         printf("%d", (decimal.bits[i] >> j) & 1);
    //     }
    //     printf(" ");
    // }
    // printf("\n");
    printf("%d %d %d %d\n",decimal.bits[0],decimal.bits[1],decimal.bits[2],decimal.bits[3]);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int exit_code = OK;

    if (!dst) {
        exit_code = ERROR;
    } else {
        s21_clear_decimal(dst);

        int sign = 0;
        if (src < 0) {
            sign = 1;
            src *= -1;
        }

        dst->bits[0] = src;
        s21_set_sign(dst, sign);
    }

    return exit_code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int exit_code = OK;

    if (!dst) {
        exit_code = ERROR;
    } else {
        s21_clear_decimal(dst);
        
        // Обработка знака
        if (src < 0) {
            dst->bits[3] |= (1 << 31); // Установка знакового бита
            src = -src; // Работаем с положительным значением
        }

        // Извлечение целой и дробной частей
        int integer = (int)src;
        float fractional = src - integer;

        // Заполнение младшей части (bits[0]) дробной части
        int fractional_as_int = (int)(fractional * pow(10, MAX_DECIMAL_PLACES));
        dst->bits[0] = fractional_as_int;

        // Заполнение средней части (bits[1]) целой части
        dst->bits[1] = integer;

        // Установка старшей части (bits[2]) в 0, так как мы не используем её для хранения
        dst->bits[2] = 0;

        // Установка показателя степени в bits[3]
        int scale = MAX_DECIMAL_PLACES; // Степень 10^7
        dst->bits[3] |= (scale & 0xFF) << 16; // Установка показателя степени в биты 16-23
    }

    return exit_code;
}

int s21_get_sign(s21_decimal src) {
    return (src.bits[3] >> 31) & 1;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst){
    int exit_code = OK;

    if (!dst){
        exit_code = ERROR;
    }else{
        *dst = 0;
        int sign = s21_get_sign(src) ? -1 : 1;
        *dst = sign * src.bits[0];
    }
    return exit_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int exit_code = OK;

    if (!dst) {
        exit_code = ERROR;
    } else {
        *dst = 0.0f; // Сброс значения
        int sign = s21_get_sign(src) ? -1 : 1; 
        int scale = (src.bits[3] >> 16) & 0xFF;
        
        // Получаем целую часть
        int int_part = src.bits[1];
        int fractional_part = src.bits[0]; 

        *dst = (float)int_part;

        if (scale > 0) {
            float fractional_as_float = (float)fractional_part / pow(10, scale);
            *dst += fractional_as_float; 
        }

        *dst *= sign;
    }

    return exit_code;
}

int s21_floor(s21_decimal value, s21_decimal *result){
    int exit_code = OK;
    s21_clear_decimal(result);

    int sign = s21_get_sign(value); // Получаем знак числа
    int scale = (value.bits[3] >> 16) & 0xFF; // Получаем степень
    int integer_part = value.bits[1]; // Целая часть
    int fractional_part = value.bits[0]; // Дробная часть

    if (sign == 0){
        result->bits[1] = integer_part;
    }else{
        if (fractional_part > 0) {
            // Если есть дробная часть, то нужно округлить вниз
            result->bits[1] = integer_part + 1; // Увеличиваем целую часть
        } else {
            // Если дробной части нет, просто копируем целую часть
            result->bits[1] = integer_part;
        }
    }

    s21_set_sign(result,sign);

    result->bits[3] = (scale & 0xFF) << 16; 

    return exit_code;

}

int s21_round(s21_decimal value, s21_decimal *result) {
    int exit_code = OK;
    s21_clear_decimal(result); 

    int sign = s21_get_sign(value); // Получаем знак числа
    int scale = (value.bits[3] >> 16) & 0xFF; // Получаем степень
    int integer_part = value.bits[1]; // Целая часть
    int fractional_part = value.bits[0]; // Дробная часть

    // Если есть дробная часть
    if (scale > 0) {
        // Приведение дробной части к целому числу
        int threshold = (int)pow(10, scale - 1); 
        if (fractional_part >= threshold) {
            integer_part += 1;
        }
    }

    // Устанавливаем целую часть в результат
    result->bits[1] = integer_part;

    // Устанавливаем знак результата
    s21_set_sign(result, sign);

    // Устанавливаем степень результата
    result->bits[3] = (scale & 0xFF) << 16; // Устанавливаем только степень

    return exit_code;
}

int main() {
    s21_decimal decimal1, decimal2, decimal3, decimal4, result;
    s21_from_float_to_decimal(3.7, &decimal1);
    s21_from_float_to_decimal(-3.7, &decimal2);
    s21_from_float_to_decimal(3.5, &decimal3);
    s21_from_float_to_decimal(-3.5, &decimal4);

    printf("Testing s21_round function:\n");

    // Тестирование положительного числа
    s21_round(decimal1, &result);
    printf("s21_round(3.7) = ");
    s21_print_decimal(result);

    // Тестирование отрицательного числа
    s21_round(decimal2, &result);
    printf("s21_round(-3.7) = ");
    s21_print_decimal(result);

    // Тестирование округления до целого
    s21_round(decimal3, &result);
    printf("s21_round(3.5) = ");
    s21_print_decimal(result);

    // Тестирование округления до целого отрицательного
    s21_round(decimal4, &result);
    printf("s21_round(-3.5) = ");
    s21_print_decimal(result);

    return 0;
}