#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct 
{
    int bits[4];
} s21_decimal;

int abs(int m);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);
int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int get_sign(s21_decimal num);
int get_scale(s21_decimal num);
int s21_negate(s21_decimal value, s21_decimal *result);
void set_sign(s21_decimal *num, int sign);
void set_scale(s21_decimal *num, int scale);
void decimal_shift(s21_decimal *num, int shift);
void s21_add(s21_decimal one_shift, s21_decimal three_shift, s21_decimal *result);
void increase_scale(s21_decimal *num, int delta);
void s21_print_decimal(s21_decimal decimal);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int main() {
    s21_decimal num1 = {2147483647, 2147483647, 2147483647, 0};
    s21_decimal num2 = {2, 0, 0, 0};
    s21_decimal res = { 0 };

    // printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    // set_scale(&num, 15);
    // set_sign(&num, 1);
    // printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    // set_scale(&num, 3);
    // set_sign(&num, -1);
    // printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    // s21_print_decimal(num);
    // increase_scale(&num, 4);
    // printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    // s21_print_decimal(num);
    set_scale(&num1, 10);
    set_scale(&num2, 5);
    printf("value_1: ");
    s21_print_decimal(num1);
    printf("value_2: ");
    s21_print_decimal(num2);
    s21_mul(num1, num2, &res);
    printf("result:\n");
    printf("sign: %d, scale: %d\n", get_sign(res), get_scale(res));
    s21_print_decimal(res);
    if (strcmp("11111111111111111111111111111111", "1111111111111111111111111111111") == 0) printf("Success\n");
    return 0;
}

int abs(int m) {
    if (m < 0) m = -m;
    return m;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    int res = 1;
    if (get_sign(num1) != get_sign(num2)) res = 0;
    for (int i = 0; i < 4 && res == 1; i++) {
        if (num1.bits[i] != num2.bits[i]) res = 0;
    }
    return res;
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
    int res = 1;
    if (s21_is_equal(num1, num2) == 1) res = 0;
    return res;
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
    int res = -1;
    if (get_sign(num1) < get_sign(num2)) res = 1;
    else if (get_sign(num1) > get_sign(num2)) res = 0;
    int compared = 0;
    for (int i = 2; i >= 0 && res == -1; i--) {
        if (num1.bits[i] < 0 && num2.bits[i] >= 0) res = 0;
        else if (num1.bits[i] >= 0 && num2.bits[i] < 0) res = 1;
        else if (num1.bits[i] < num2.bits[i]) res = 1;
        else if (num1.bits[i] > num2.bits[i]) res = 0;
    }
    if (res == -1) res = 0;
    return res;
}

int s21_is_greater(s21_decimal num1, s21_decimal num2) {
    int res = 1;
    if (s21_is_less(num1, num2) == 1) res = 0;
    else if (s21_is_equal(num1, num2) == 1) res = 0;
    return res;
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
    int res = 0;
    if (s21_is_less(num1, num2) == 1 || s21_is_equal(num1, num2) == 1) res = 1;
    return res;
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
    int res = 0;
    if (s21_is_greater(num1, num2) == 1 || s21_is_equal(num1, num2) == 1) res = 1;
    return res;
}

int get_sign(s21_decimal num) {
    int res;
    int sign = (num.bits[3] >> 31) & 1;
    if (sign == 1) res = -1;
    else res = 1;
    return res;
}

int get_scale(s21_decimal num) {
    int scale = 0;
    for (int i = 16, j = 0; i < 24; i++, j++) {
        int bit = (num.bits[3] >> i) & 1;
        if (bit == 1) scale += (int)pow(2, j);
    }
    return scale;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    for (int i = 0; i < 4; i++) {
        result->bits[i] = value.bits[i];
    }
    if (get_sign(value) == 1) set_sign(result, -1);
    else set_sign(result, 1);
    return 0;
}

void set_scale(s21_decimal *num, int scale) {
    if (scale >= 0 && scale <= 28) {
        int sign = get_sign(*num);
        num->bits[3] = 0;
        num->bits[3] |= (scale << 16);
        set_sign(num, sign);
    }
}

void set_sign(s21_decimal *num, int sign) {
    if (sign == -1 && get_sign(*num) == 1) num->bits[3] |= (1 << 31);
    else if (sign == 1 && num->bits[3] < 0) {
        int scale = get_scale(*num);
        num->bits[3] = 0;
        num->bits[3] |= (scale << 16);
    }
    else if (sign == 1 && num->bits[3] == 0) num->bits[3] = 0;
}

// увеличение дробной части на delta знаков
void increase_scale(s21_decimal *num, int delta) {
    s21_decimal result = *num;
    for (int i = 0; i < delta; i++) {
        s21_decimal one_shift = result;
        s21_decimal three_shift = result;
        decimal_shift(&one_shift, 1);
        decimal_shift(&three_shift, 3);
        s21_add(one_shift, three_shift, &result);
    }
    int scale = get_scale(*num);
    *num = result;
    set_scale(num, scale + delta);
}

void decimal_shift(s21_decimal *num, int shift) {
    int buffer1 = 0;
    for (int i = 0; i < shift; i++) {
        buffer1 = buffer1 << 1;
        buffer1 |= (num->bits[0] >> 31 - i) & 1;
    }
    num->bits[0] = num->bits[0] << shift;
    int buffer2 = 0;
    for (int i = 0; i < shift; i++) {
        buffer2 = buffer2 << 1;
        buffer2 |= (num->bits[1] >> 31 - i) & 1;
    }
    num->bits[1] = num->bits[1] << shift;
    num->bits[1] |= buffer1;
    num->bits[2] = num->bits[2] << shift;
    num->bits[2] |= buffer2;
}


// Целое + целое
void s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int buffer = 0;
    for (int i = 0; i < 3; i++) {
        result->bits[i] = 0;
        for (int j = 0; j <= 31; j++) {
            int num1 = (value_1.bits[i] >> j) & 1;
            int num2 = (value_2.bits[i] >> j) & 1;
            if ((num1 + num2 + buffer) == 3) {
                result->bits[i] |= (1 << j);
                buffer = 1;
            } else if ((num1 + num2 + buffer) == 2) {
                buffer = 1;
            } else if ((num1 + num2 + buffer) == 1) {
                result->bits[i] |= (1 << j);
                buffer = 0;
            } else buffer = 0;
        }
    }
}

void s21_print_decimal(s21_decimal decimal) {
    for (int i = 2; i >= 0; i--) {
        for (int j = 31; j >= 0; j--) {
            printf("%d", (decimal.bits[i] >> j) & 1);
        }
    }
    printf("\n");
}

// Лучшее на данный момент (умножение)
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal buffer_dec = { 0 };
    set_scale(result, get_scale(value_1) + get_scale(value_2));
    if ((get_sign(value_1) == -1 && get_sign(value_2) == 1) || (get_sign(value_1) == 1 && get_sign(value_2) == -1)) set_sign(result, -1);
    // берем каждый бит value_2
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j <= 31; j++) {
            int bit2 = (value_2.bits[i] >> j) & 1;
            s21_decimal buffer_line = { 0 };
            // берем каждый бит value_1
            for (int m = 0; m < 3 && bit2 == 1; m++) {
                for (int n = 0; n <= 31; n++) {
                    int bit1 = (value_1.bits[m] >> n) & 1;
                    buffer_line.bits[m] |= (bit1 << n);
                }
            }
            decimal_shift(&buffer_line, j + 31*i);
            s21_add(buffer_line, buffer_dec, result);
            buffer_dec = *result;
        }
    }
    *result = buffer_dec;
}

// int bits_cmp(int num1, int num2, int scale1, int scale2) {
//     int res = 0;
//     for (int i = 0; i < 32; i++) {
//         if (scale1 == i );
//     }
//     return res;
// }