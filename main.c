#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"
int main(void) {
    s21_decimal num1;
    s21_decimal num2;
    s21_decimal num3;

    s21_from_int_to_decimal(-15, &num1);
    s21_from_int_to_decimal(INT_MIN, &num2);
    s21_from_int_to_decimal(INT_MAX, &num3);
    
    s21_print_decimal(num1);
    s21_print_decimal(num2);
    s21_print_decimal(num3);
    return 0;
}