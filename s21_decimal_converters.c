#include "s21_decimal_converters.h"

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