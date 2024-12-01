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
        
        ///////////////////////
    }

    return exit_code;
}
