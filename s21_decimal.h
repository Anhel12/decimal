#ifndef S21_DECIMAL
#define S21_DECIMAL
#include <stdio.h>

#include "s21_helpers.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
#endif
