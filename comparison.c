#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct 
{
    int bits[4];
} s21_decimal;

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
void decimal_shift_right(s21_decimal *num, int shift);
int s21_add(s21_decimal one_shift, s21_decimal three_shift, s21_decimal *result);
void increase_scale(s21_decimal *num, int delta);
void decrease_scale(s21_decimal *num, int delta);
void s21_print_decimal(s21_decimal decimal);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_same_scale(s21_decimal *tmp1, s21_decimal *tmp2);
int check_for_verylow(s21_decimal *num, int new_scale);
void summ_or_sub(s21_decimal value_1, s21_decimal value_2, int *res_sign, s21_decimal *tmp1, s21_decimal *tmp2, s21_decimal *result, int *res, int *summ);
int check_for_zero(s21_decimal num);

int main() {
    s21_decimal num1 = {590, 500, -1, 0};
    s21_decimal num2 = {600, 500, -1, 0};
    s21_decimal res = { 0 };
    
    printf("value_1: ");
    set_scale(&num1, 0);
    set_sign(&num1, 1);
    printf("sign: %d, scale: %d\n", get_sign(num1), get_scale(num1));
    s21_print_decimal(num1);
    printf("value_2: ");
    set_scale(&num2, 0);
    set_sign(&num2, 1);
    printf("sign: %d, scale: %d\n", get_sign(num2), get_scale(num2));
    s21_print_decimal(num2);
    //printf("%d\n", s21_is_equal(num1, num2));
    // set_scale(&num1, 15);
    // decrease_scale(&num1, 1);
    int fault = s21_sub(num1, num2, &res);
    //increase_scale(&num2, 28);
    printf("result: %d\n", fault);
    printf("sign: %d, scale: %d\n", get_sign(res), get_scale(res));
	//printf("%d\n", s21_is_greater_or_equal(num1, num2));
    s21_print_decimal(res);

    //if (strcmp("1111111111111111111111111111111100000000000000000000000000000000", "111111111111111111111111111111110000000000000000000000000000000") == 0) printf("Success\n");
    return 0;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    int res = 1;
    if (check_for_zero(num1) && check_for_zero(num2)) res = 1;
    else if (get_sign(num1) != get_sign(num2)) res = 0;
    s21_decimal tmp1 = num1;
    s21_decimal tmp2 = num2;
    if ((get_scale(tmp1) != get_scale(tmp2)) && res == 1) set_same_scale(&tmp1, &tmp2);
    for (int i = 0; i < 3 && res == 1; i++) {
        if (((tmp1.bits[i] << 31) & 1) != ((tmp1.bits[i] << 31) & 1)) res = 0;
        else if (tmp1.bits[i] != tmp2.bits[i]) res = 0;
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
    if (check_for_zero(num1) && check_for_zero(num2)) res = 0;
    else if (get_sign(num1) < get_sign(num2)) res = 1;
    else if (get_sign(num1) > get_sign(num2)) res = 0;
    s21_decimal tmp1 = num1;
    s21_decimal tmp2 = num2;
    if (get_scale(tmp1) != get_scale(tmp2) && res == -1) set_same_scale(&tmp1, &tmp2);
    for (int i = 2; i >= 0 && res == -1; i--) {
		if (((tmp1.bits[i] << 31) & 1) > ((tmp1.bits[i] << 31) & 1)) res = 0;
		else if (((tmp1.bits[i] << 31) & 1) < ((tmp1.bits[i] << 31) & 1)) res = 1;
        else if (tmp1.bits[i] < 0 && tmp2.bits[i] >= 0) res = 0;
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

void decimal_shift_right(s21_decimal *num, int shift) {
    int buffer1 = 0;
    for (int i = 0; i < shift; i++) {
        buffer1 = buffer1 << 1;
        buffer1 |= (num->bits[2] >> i) & 1;
    }
    num->bits[2] = num->bits[2] >> shift;
    int buffer2 = 0;
    for (int i = 0; i < shift; i++) {
        buffer2 = buffer2 << 1;
        buffer2 |= (num->bits[1] >> 31) & 1;
    }
    num->bits[1] = num->bits[1] << 31 - shift;
    num->bits[1] |= buffer1;
    num->bits[0] = num->bits[0] << 31 - shift;
    num->bits[0] |= buffer2;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int res = 0;
    s21_decimal tmp1 = value_1;
    s21_decimal tmp2 = value_2;
    if (get_sign(value_1) == get_sign(value_2)) {
        int buffer = 0;
        set_same_scale(&tmp1, &tmp2);
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
        set_scale(result, get_scale(tmp1));
        if (buffer == 1 && get_scale(*result) > 0) {
            decrease_scale(&tmp1, 1);
            decrease_scale(&tmp2, 1);
            s21_add(tmp1, tmp2, result);
        } else if (buffer == 1 && get_scale(*result) == 0 && get_sign(value_1) == 1) res = 1;
        else if (buffer == 1 && get_scale(*result) == 0 && get_sign(value_1) == -1) res = 2;
        set_sign(result, get_sign(value_1));
    } else {
        int sign1 = get_sign(tmp1);
        int sign2 = get_sign(tmp2);
        set_sign(&tmp1, 1);
        set_sign(&tmp2, 1);
        if (s21_is_greater_or_equal(tmp1, tmp2)) {
            if (sign1 > sign2) {
                res = s21_sub(tmp1, tmp2, result);
                set_sign(result, 1);
            } else {
                res = s21_sub(tmp2, tmp1, result);
                set_sign(result, -1);
            }
        } else {
           if (sign1 > sign2) {
                res = s21_sub(tmp2, tmp1, result);
                set_sign(result, -1);
            } else {
                res = s21_sub(tmp1, tmp2, result);
                set_sign(result, 1);
            }
        }
    }
    return res;
}

void s21_print_decimal(s21_decimal decimal) {
    for (int i = 2; i >= 0; i--) {
        for (int j = 31; j >= 0; j--) {
            printf("%d", (decimal.bits[i] >> j) & 1);
        }
    }
    printf("\n");
}

// Без обработки ошибок
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int res = 0;
    int summ_shift = 0;
    int shift = 0;
    s21_decimal buffer_dec = { 0 };
    s21_decimal tmp1 = value_1;
    s21_decimal tmp2 = value_2;
    int new_scale = get_scale(value_1) + get_scale(value_2);
    if ((get_sign(value_1) == -1 && get_sign(value_2) == 1) || (get_sign(value_1) == 1 && get_sign(value_2) == -1)) set_sign(result, -1);
    for (int i = 0; i < 3 && res == 0; i++) {
        for (int j = 0; j <= 31 && res == 0; j++) {
            int bit2 = (value_2.bits[i] >> j) & 1;
            s21_decimal buffer_line = { 0 };
            if (bit2 == 1) shift = 0;
            for (int m = 0; m < 3 && bit2 == 1; m++) {
                for (int n = 0; n <= 31; n++) {
                    int bit1 = (value_1.bits[m] >> n) & 1;
                    buffer_line.bits[m] |= (bit1 << n);
                }
            }
            int k = 0;
            while (k < (j + 31*i) && !((buffer_line.bits[2] << 31) & 1) && bit2 == 1) {
                decimal_shift_left(&buffer_line, 1);
                k++;
            }
            if (k < (j + 31*i) && bit2 == 1) {
                shift = (j + 31*i) - k - summ_shift;
                summ_shift += shift;
                decimal_shift_right(&buffer_dec, shift);
            }
            s21_add(buffer_line, buffer_dec, result);
            buffer_dec = *result;
        }
    }
    if (new_scale > 28 && res == 0 && check_for_verylow(&buffer_dec, new_scale - 28)) res = 2;
    else if (new_scale < 0) res = 1;
    if (new_scale <= 28) set_scale(&buffer_dec, new_scale);
    else set_scale(&buffer_dec, 28);
    *result = buffer_dec; 
    return res;
}

// Уменьшение дробной части на delta знаков (деление на 10 в delta степени)
void decrease_scale(s21_decimal *num, int delta) {
    int k = 0;
	int zero;
    while (k < delta) {
        s21_decimal buffer1 = { 0 };
        s21_decimal buffer2 = { 0 };
        s21_decimal res = { 0 };
        s21_decimal del = {10, 0, 0, 0};
        int j = 93;
        int i = 2;
        // Получаем первое делимое
        while (buffer1.bits[0] < del.bits[0] && j >= 0) {
            decimal_shift_left(&buffer1, 1);
            buffer1.bits[0] |= (num->bits[i] >> (j - 31*i)) & 1;
            j--;
            if (j % 31 == 0) i--;
        }
		if (buffer1.bits[0] == 0 && buffer1.bits[1] == 0 && buffer1.bits[2] == 0) zero = 1;
		else zero = 0;
        // Деление в столбик
        while (j >= 0 && zero == 0) {
            if (buffer1.bits[0] >= del.bits[0]) {
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
        if (!s21_is_less(buffer1, del) && zero == 0) res.bits[0] |= 1;
        for (int m = 0; m < 3; m++) num->bits[m] = res.bits[m];
        k++;
    }
    set_scale(num, get_scale(*num) - delta);
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int res = 0;
    int buffer = 0;
    int summ = 0;
    int res_sign = 0;
    s21_decimal tmp1 = { 0 };
    s21_decimal tmp2 = { 0 };
    summ_or_sub(value_1, value_2, &res_sign, &tmp1, &tmp2, result, &res, &summ);
    if (!summ) set_same_scale(&tmp1, &tmp2);
    for (int i = 0; i < 3 && !summ; i++) {
        result->bits[i] = 0;
        for (int j = 0; j <= 31; j++) {
            int num1 = ((tmp1.bits[i] >> j) & 1);
            int num2 = ((tmp2.bits[i] >> j) & 1);
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
    if (!summ) set_scale(result, get_scale(tmp1));
    set_sign(result, res_sign);
    return res;
}

void set_same_scale(s21_decimal *tmp1, s21_decimal *tmp2) {
    if ((get_scale(*tmp1)) != (get_scale(*tmp2))) {
        int delta = get_scale(*tmp1) - get_scale(*tmp2);
        while (delta < 0 && (((tmp1->bits[2]) >> 31) & 1) == 0 && (get_scale(*tmp1) < 28)) {
            increase_scale(tmp1, 1);
            delta++;
        }
        while (delta < 0) {
            decrease_scale(tmp2, 1);
            delta++;
        }
        while (delta > 0 && (((tmp2->bits[2]) >> 31) & 1) == 0 && (get_scale(*tmp2) < 28)) {
            increase_scale(tmp2, 1);
            delta--;
        }
        while (delta > 0) {
            decrease_scale(tmp1, 1);
            delta--;
        }
    }
}

int check_for_verylow(s21_decimal *num, int delta_scale) {
    int verylow = 1;
	decrease_scale(num, delta_scale);
	for (int i = 2; i >= 0 && verylow == 1; i--) {
		for (int j = 0; j < 32 && verylow == 1; j++) {
			if ((num->bits[i] >> j) & 1) verylow = 0;
		}
	}
	return verylow;
}

// Для вычитания
void summ_or_sub(s21_decimal value_1, s21_decimal value_2, int *res_sign, s21_decimal *tmp1, s21_decimal *tmp2, s21_decimal *result, int *res, int *summ) {
    if (!((value_1.bits[3] >> 31) & 1) && !((value_2.bits[3] >> 31) & 1) && s21_is_greater_or_equal(value_1,value_2)) {
        *res_sign = 1;
        *tmp1 = value_1;
        *tmp2 = value_2;
    } else if (!((value_1.bits[3] >> 31) & 1) && !((value_2.bits[3] >> 31) & 1) && !s21_is_greater_or_equal(value_1,value_2)) {
        *res_sign = -1;
        *tmp1 = value_2;
        *tmp2 = value_1;
    } else if (((value_1.bits[3] >> 31) & 1) && !((value_2.bits[3] >> 31) & 1)) {
        *tmp1 = value_1;
        *tmp2 = value_2;
        set_sign(tmp1, 1);
        *res_sign = -1;
        *res = s21_add(*tmp1, *tmp2, result);
        if (*res == 1) *res = 2;
        *summ = 1;
    } else if (!((value_1.bits[3] >> 31) & 1) && ((value_2.bits[3] >> 31) & 1)) {
        *tmp1 = value_1;
        *tmp2 = value_2;
        set_sign(tmp2, 1);
        *res_sign = 1;
        *res = s21_add(*tmp1, *tmp2, result);
        *summ = 1;
    } else if (((value_1.bits[3] >> 31) & 1) && ((value_2.bits[3] >> 31) & 1)) {
        *tmp1 = value_1;
        *tmp2 = value_2;
        set_sign(tmp1, 1);
        set_sign(tmp2, 1);
        if (s21_is_greater_or_equal(*tmp1, *tmp2)) {
            *res_sign = -1;
        } else {
            *res_sign = 1;
            s21_decimal temp = *tmp1;
            tmp1 = tmp2;
            *tmp2 = temp;
        }
    }
}

int check_for_zero(s21_decimal num) {
    int res = 1;
    for (int i = 0; i < 3 && res; i++) {
        for (int j = 0; j < 32 && res; j++) {
            if (((num.bits[i] >> j) & 1) == 1) res = 0;
        }
    }
    return res;
}