#include"s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result){
    int exit_code = OK;
    s21_clear_decimal(result);

    int sign = s21_get_sign(value); // Получаем знак числа
    int scale = (value.bits[3] >> 16) & 0xFF; // Получаем степень
    int integer_part = value.bits[1]; // Целая часть
    int fractional_part = value.bits[0]; // Дробная часть

    if (sign == 0){
        result->bits[1] = integer_part;
    }else{
        if (fractional_part > 0) {
            // Если есть дробная часть, то нужно округлить вниз
            result->bits[1] = integer_part + 1; // Увеличиваем целую часть
        } else {
            // Если дробной части нет, просто копируем целую часть
            result->bits[1] = integer_part;
        }
    }

    s21_set_sign(result,sign);

    result->bits[3] = (scale & 0xFF) << 16; 

    return exit_code;

}

int s21_round(s21_decimal value, s21_decimal *result) {
    int exit_code = OK;
    s21_clear_decimal(result); 

    int sign = s21_get_sign(value); // Получаем знак числа
    int scale = (value.bits[3] >> 16) & 0xFF; // Получаем степень
    int integer_part = value.bits[1]; // Целая часть
    int fractional_part = value.bits[0]; // Дробная часть

    // Если есть дробная часть
    if (scale > 0) {
        // Приведение дробной части к целому числу
        int threshold = (int)pow(10, scale - 1); 
        if (fractional_part >= threshold) {
            integer_part += 1;
        }
    }

    // Устанавливаем целую часть в результат
    result->bits[1] = integer_part;

    // Устанавливаем знак результата
    s21_set_sign(result, sign);

    // Устанавливаем степень результата
    result->bits[3] = (scale & 0xFF) << 16; // Устанавливаем только степень

    return exit_code;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int exit_code = OK;
    s21_clear_decimal(result); // Очистка результата

    // Получение знака и степени
    int sign = s21_get_sign(value); // Получаем знак числа
    int scale = (value.bits[3] >> 16) & 0xFF; // Получаем степень
    int integer_part = value.bits[1]; // Целая часть

    // Устанавливаем целую часть в результат
    result->bits[1] = integer_part;

    // Устанавливаем знак результата
    s21_set_sign(result, sign);

    // Устанавливаем степень результата в 0, так как дробная часть отбрасывается
    result->bits[3] = (0 & 0xFF) << 16; // Устанавливаем степень в 0

    return exit_code;
}