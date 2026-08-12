#include <stdint.h>
#include <stdio.h>
#ifndef HELPERS
#define HELPERS
typedef struct {
  unsigned int bits[4];
} s21_decimal;

// метод для получения конкретного бита
int s21_get_bit(s21_decimal d, int index);
// метод для установки бита
void s21_set_bit(s21_decimal *d, int index, int value);
// метод для обнуления битов
void s21_zero_decimal(s21_decimal *d);
// метод для получения знака
int s21_get_sign(s21_decimal d);
// метод для установки знака
void s21_set_sign(s21_decimal *d, int sign);
// метод для получения точности
int s21_get_scale(s21_decimal d);
// метод для установки точности
void s21_set_scale(s21_decimal *d, int scale);
// проверка равно ли число нулю
int s21_is_zero(s21_decimal d);
// проверка является ли число корректным
int s21_is_valid(s21_decimal d);
// сравнивает мантиссы a < b = -1 | a > b = 1 | a == b = 0
int s21_compare_mantissa(s21_decimal a, s21_decimal b);
// приведение к одной точности
int s21_normalize(s21_decimal *d1, s21_decimal *d2);
// банковское окргуление, даже близко не то же самое что и round! Валера если ты
// читаешь это, не получится просто переименовать метод они делают разное!!!
int s21_bank_round(s21_decimal *d, int target_scale);
// делим мантиссу на 10
int s21_div_mantissa_by_10(s21_decimal *d);
// добавляем единицу к мантиссе
int s21_add_mantissa_one(s21_decimal *d);
// сдвиг бита влево с переносом
int s21_shift_left(s21_decimal *d);
// быстрый метод для вычитания мантисс
// не использовал тот что есть потому что этому плевать на точность и знаки
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result);
// быстрый метод сложения мантисс
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result);
// Я ДОЛГО ДУМАЛ , НО ВСЁ ТАКИ РЕШИЛ ДОБАВИТЬ ЭТИ МЕТОДЫ (ВАЛЕРА ТЕБЕ СКОРЕЕ
// ВСЕГО ПРИГОДИТСЯ)
//  домножение мантиссы на 10
int s21_mul_mantissa_by_10(s21_decimal *d);
// делим мантиссу на 10
int s21_div_mantissa_by_10(s21_decimal *d);
// добавляем единицу к мантиссе
int s21_add_mantissa_one(s21_decimal *d);

#endif