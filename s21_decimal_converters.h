#ifndef S21_DECIMAL_CONVERTERS_H
#define S21_DECIMAL_CONVERTERS_H

#include <limits.h>

#include "s21_types.h"
#include "s21_helpers.h"

#define OK 0
#define ERROR 1
#define MAX_DECIMAL_PLACES 7

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);


#endif // S21_DECIMAL_CONVERTERS_H
