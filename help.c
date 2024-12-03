#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fractional_part_as_integer(double number) {
    // Преобразуем число в строку
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.10f", number); // Используем 10 знаков после запятой

    // Находим позицию запятой
    char *decimal_point = strchr(buffer, '.');
    if (decimal_point != NULL) {
        // Получаем дробную часть как строку
        return atoi(decimal_point + 1); // Преобразуем дробную часть в целое число
    }
    return 0; // Если запятой нет, возвращаем 0
}

int main() {
    double num;
    
    printf("Введите число: ");
    scanf("%lf", &num);
    
    int frac_part = fractional_part_as_integer(num);
    
    printf("Дробная часть числа как целое: %d\n", frac_part);
    
    return 0;
}