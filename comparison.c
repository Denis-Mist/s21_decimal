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
void decimal_shift_left(s21_decimal *num, int shift);
void s21_add(s21_decimal one_shift, s21_decimal three_shift, s21_decimal *result);
void increase_scale(s21_decimal *num, int delta);
void decrease_scale(s21_decimal *num, int delta);
void s21_print_decimal(s21_decimal decimal);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_same_scale(s21_decimal *tmp1, s21_decimal *tmp2);

int main() {
    s21_decimal num1 = {1700000000, 0, 0, 0};
    s21_decimal num2 = {17000, 0, 0, 0};
    s21_decimal res = { 0 };

    // set_scale(&num1, 10);
    printf("value_1: ");
    set_scale(&num1, 6);
    s21_print_decimal(num1);
    printf("value_2: ");
    set_scale(&num2, 1);
    s21_print_decimal(num2);
    printf("%d\n", s21_is_equal(num1, num2));
    // set_scale(&num1, 15);
    // decrease_scale(&num1, 1);
    // printf("result:\n");
    // printf("sign: %d, scale: %d\n", get_sign(num1), get_scale(num1));
    // s21_print_decimal(num1);
    //if (strcmp("", "") == 0) printf("Success\n");
    return 0;
}

int abs(int m) {
    if (m < 0) m = -m;
    return m;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    int res = 1;
    if (get_sign(num1) != get_sign(num2)) res = 0;
    s21_decimal tmp1 = num1;
    s21_decimal tmp2 = num2;
    set_same_scale(&tmp1, &tmp2);
    for (int i = 0; i < 4 && res == 1; i++) {
        if (tmp1.bits[i] != tmp2.bits[i]) res = 0;
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
    s21_decimal tmp1 = num1;
    s21_decimal tmp2 = num2;
    set_same_scale(&tmp1, &tmp2);
    for (int i = 2; i >= 0 && res == -1; i--) {
        if (tmp1.bits[i] < 0 && tmp2.bits[i] >= 0) res = 0;
        else if (tmp1.bits[i] >= 0 && tmp2.bits[i] < 0) res = 1;
        else if (tmp1.bits[i] < tmp2.bits[i]) res = 1;
        else if (tmp1.bits[i] > tmp2.bits[i]) res = 0;
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

// увеличение дробной части на delta знаков (умножение на 10 в delta степени)
void increase_scale(s21_decimal *num, int delta) {
    s21_decimal result = *num;
    for (int i = 0; i < delta; i++) {
        s21_decimal one_shift = result;
        s21_decimal three_shift = result;
        decimal_shift_left(&one_shift, 1);
        decimal_shift_left(&three_shift, 3);
        s21_add(one_shift, three_shift, &result);
    }
    int scale = get_scale(*num);
    *num = result;
    set_scale(num, scale + delta);
}

void decimal_shift_left(s21_decimal *num, int shift) {
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
            decimal_shift_left(&buffer_line, j + 31*i);
            s21_add(buffer_line, buffer_dec, result);
            buffer_dec = *result;
        }
    }
    *result = buffer_dec;
}

// Уменьшение дробной части на delta знаков (деление на 10 в delta степени)
void decrease_scale(s21_decimal *num, int delta) {
    int k = 0;
    while (k < delta) {
        s21_decimal buffer1 = { 0 };
        s21_decimal buffer2 = { 0 };
        s21_decimal res = { 0 };
        s21_decimal del = {10, 0, 0, 0};
        int j = 93;
        int i = 2;
        // Получаем первое делимое
        while (s21_is_less(buffer1, del)) {
            decimal_shift_left(&buffer1, 1);
            buffer1.bits[0] |= (num->bits[i] >> (j - 31*i)) & 1;
            j--;
            if (j % 31 == 0) i--;
        }
        // Деление в столбик
        while (j >= 0) {
            if (!s21_is_less(buffer1, del)) {
                res.bits[0] |= 1;
                s21_sub(buffer1, del, &buffer2);
                buffer1 = buffer2;
                if (j != 0) decimal_shift_left(&res, 1);
            } else {
                decimal_shift_left(&buffer1, 1);
                buffer1.bits[0] |= (num->bits[i] >> (j - 31*i)) & 1;
                j--;
                if (j % 31 == 0) i--;
                if (s21_is_less(buffer1, del) && j != 0) decimal_shift_left(&res, 1);
            }
        }
        if (!s21_is_less(buffer1, del)) res.bits[0] |= 1;
        for (int m = 0; m < 3; m++) num->bits[m] = res.bits[m];
        k++;
    }
    set_scale(num, get_scale(*num) - delta);
}

// Большее целое - меньшее целое
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int buffer = 0;
    
    for (int i = 0; i < 3; i++) {
        result->bits[i] = 0;
        for (int j = 0; j <= 31; j++) {
            int num1 = (value_1.bits[i] >> j) & 1;
            int num2 = (value_2.bits[i] >> j) & 1;
            if ((num1 - num2 - buffer) == 1) {
                result->bits[i] |= (1 << j);
                buffer = 0;
            } else if ((num1 - num2 - buffer) == -1) {
                result->bits[i] |= (1 << j);
                buffer = 1;
            } else if ((num1 - num2 - buffer) == -2) {
                buffer = 1;
            } else {
                buffer = 0;
            }
        }
    }
}

void set_same_scale(s21_decimal *tmp1, s21_decimal *tmp2) {
    if (get_scale(*tmp1) != get_scale(*tmp2)) {
        int delta = get_scale(*tmp1) - get_scale(*tmp2);
        while (delta < 0 && (((tmp1->bits[2]) >> 31) & 1) == 0 && get_scale(*tmp1) < 28) {
            increase_scale(tmp1, 1);
            delta++;
        }
        while (delta < 0) {
            decrease_scale(tmp2, 1);
            delta++;
        }
        while (delta > 0 && (((tmp2->bits[2]) >> 31) & 1) == 0 && get_scale(*tmp2) < 28) {
            increase_scale(tmp2, 1);
            delta--;
        }
        while (delta > 0) {
            decrease_scale(tmp1, 1);
            delta--;
        }
    }
}
