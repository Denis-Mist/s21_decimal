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
    for (int i = 3; i >= 0; i--) {
        for (int j = 31; j >= 0; j--) {
            printf("%d", (decimal.bits[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
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


int main(void) {
    s21_decimal num1;
    s21_decimal num2;
    s21_decimal num3;

    s21_from_int_to_decimal(-15, &num1);
    s21_from_int_to_decimal(INT_MIN, &num2);
    s21_from_int_to_decimal(INT_MAX, &num3);

    int dst1;

    s21_from_decimal_to_int(num3,&dst1);
    printf("%d",dst1);

    // s21_from_float_to_decimal(-15.34,&num1);
    // s21_from_float_to_decimal(0.231,&num2);
    
    // s21_print_decimal(num1);
    // s21_print_decimal(num2);
    // s21_print_decimal(num3);
    return 0;
}