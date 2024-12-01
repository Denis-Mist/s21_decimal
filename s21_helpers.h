#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdio.h>

#include "s21_types.h"

void s21_clear_decimal(s21_decimal *decimal);
void s21_set_sign(s21_decimal *decimal, int sign);

void s21_print_decimal(s21_decimal decimal);

#endif // S21_HELPERS_H