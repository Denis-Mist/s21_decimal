#include "s21_helpers.h"

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

int s21_get_sign(s21_decimal src) {
    return (src.bits[3] >> 31) & 1;
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