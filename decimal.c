#include <stdio.h>
#include <math.h>

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

int main() {
    s21_decimal num = {15, -38, 18, 0};
    // s21_decimal num2 = {15, -35, 18, 7};
    // int res;
    // res = s21_is_equal(num1, num2);
    // printf("%d ", res);
    // res = s21_is_not_equal(num1, num2);
    // printf("%d ", res);
    // res = s21_is_less(num1, num2);
    // printf("%d ", res);
    // res = s21_is_greater(num1, num2);
    // printf("%d\n", res);
    // printf("%d %d\n", get_sign(num1), get_sign(num2));
    printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    set_scale(&num, 15);
    set_sign(&num, 1);
    printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
    set_scale(&num, 3);
    set_sign(&num, 1);
    printf("sign: %d, scale: %d\n", get_sign(num), get_scale(num));
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
        if (num1.bits[i] < 0 && num2.bits >= 0) res = 0;
        else if (num1.bits[i] >= 0 && num2.bits < 0) res = 1;
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
        int temp = (num.bits[3] >> i) & 1;
        if (temp == 1) scale += (int)pow(2, j);
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

// int bits_cmp(int num1, int num2, int scale1, int scale2) {
//     int res = 0;
//     for (int i = 0; i < 32; i++) {
//         if (scale1 == i );
//     }
//     return res;
// }