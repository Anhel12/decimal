#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s21_helpers.h"

// ПРОТОТИПЫ
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return 1;
  }
  if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
    s21_zero_decimal(result);
    return 0;
  }
  if (s21_is_zero(value_1)) {
    *result = value_2;
    return 0;
  } else if (s21_is_zero(value_2)) {
    *result = value_1;
    return 0;
  }
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  // Обработка разных знаков
  if (sign1 != sign2) {
    // a + (-b) = a - b, где b - положительное
    if (sign2 == 1) {
      s21_set_sign(&value_2, 0); // делаем value_2 положительным
      return s21_sub(value_1, value_2, result);
    } else {
      // (-a) + b = b - a, где a - положительное
      s21_set_sign(&value_1, 0); // делаем value_1 положительным
      return s21_sub(value_2, value_1, result);
    }
  }

  // сложение
  if (s21_normalize(&value_1, &value_2)) {
    return 1;
  }

  int scale = s21_get_scale(value_1);
  s21_decimal temp_result;
  s21_zero_decimal(&temp_result);

  uint64_t carry = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t temp = (uint64_t)(uint32_t)value_1.bits[i] +
                    (uint64_t)(uint32_t)value_2.bits[i] + carry;
    temp_result.bits[i] = (uint32_t)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }

  // Обработка переполнения с банковским округлением
  while ((carry != 0 || (temp_result.bits[2] & 0x80000000)) && scale > 0) {
    // Уменьшаем масштаб и применяем банковское округление
    if (s21_bank_round(&temp_result, scale - 1)) {
      return 1;
    }
    scale--;
    // Проверяем, не появилось ли новое переполнение после округления
    carry = 0;
    // Проверяем, не стало ли число слишком большим
    if (temp_result.bits[2] & 0x80000000) {
      // Если после округления всё ещё переполнение, продолжаем цикл
      continue;
    }
    break;
  }

  // Если после всех попыток уменьшить масштаб всё ещё переполнение
  if (carry != 0 || (temp_result.bits[2] & 0x80000000)) {
    return 2; // overflow
  }

  // Копируем результат
  *result = temp_result;
  s21_set_scale(result, scale);
  s21_set_sign(result, sign1);

  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return 1;
  }

  // Проверка на нули
  if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
    s21_zero_decimal(result);
    return 0;
  }
  if (s21_is_zero(value_1)) {
    *result = value_2;
    s21_set_sign(result, !s21_get_sign(value_2));
    return 0;
  }
  if (s21_is_zero(value_2)) {
    *result = value_1;
    return 0;
  }

  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  if (sign1 != sign2) {
    // a - (-b) = a + b
    if (sign2 == 1) {
      s21_set_sign(&value_2, 0); // делаем value_2 положительным
      return s21_add(value_1, value_2, result);
    } else {
      // (-a) - b = -(a + b)
      s21_set_sign(&value_1, 0); // делаем value_1 положительным
      int code = s21_add(value_1, value_2, result);
      s21_set_sign(result, 1); // результат отрицательный
      return code;
    }
  }

  // Дальше работаем только с числами одного знака
  if (s21_normalize(&value_1, &value_2)) {
    return 1;
  }

  int scale = s21_get_scale(value_1);
  int cmp = s21_compare_mantissa(value_1, value_2);

  if (cmp == 0) {
    s21_zero_decimal(result);
    return 0;
  }

  s21_decimal bigger;
  s21_decimal smaller;
  int res_sign;

  if (cmp == 1) {
    bigger = value_1;
    smaller = value_2;
    res_sign = sign1; // знак результата = знак первого числа
  } else {
    bigger = value_2;
    smaller = value_1;
    res_sign = !sign1; // знак результата = противоположный знаку первого
  }

  // Вычитание мантисс
  uint64_t borrow = 0;
  s21_decimal temp_result;
  s21_zero_decimal(&temp_result);

  for (int i = 0; i < 3; i++) {
    uint64_t temp = (uint64_t)(uint32_t)bigger.bits[i] -
                    (uint64_t)(uint32_t)smaller.bits[i] - borrow;
    temp_result.bits[i] = (uint32_t)(temp & 0xFFFFFFFF);
    borrow = (temp >> 32) & 1;
  }

  if (borrow != 0) {
    // Если заём есть - нужно уменьшить масштаб и применить банковское
    // округление
    while (borrow != 0 && scale > 0) {
      // Увеличиваем мантиссу, чтобы компенсировать уменьшение масштаба
      // Но так как у нас отрицательный результат, нужно сделать обратное
      // При вычитании с заёмом - это значит, что результат отрицательный
      // и его нужно нормализовать
      s21_decimal temp = temp_result;
      if (s21_bank_round(&temp, scale - 1)) {
        return 1;
      }
      temp_result = temp;
      scale--;
      // Проверяем, не исчез ли заём
      borrow = 0;
      // Проверяем, не стало ли число слишком большим
      if (temp_result.bits[2] & 0x80000000) {
        borrow = 1;
      }
    }

    if (borrow != 0) {
      return 2; // underflow
    }
  }

  *result = temp_result;
  s21_set_scale(result, scale);
  s21_set_sign(result, res_sign);

  return 0;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return 1;
  }

  if (s21_is_zero(value_1) || s21_is_zero(value_2)) {
    s21_zero_decimal(result);
    return 0;
  }
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int result_sign = (sign1 != sign2) ? 1 : 0;
  s21_set_sign(&value_1, 0);
  s21_set_sign(&value_2, 0);
  int scale = s21_get_scale(value_1) + s21_get_scale(value_2);
  s21_zero_decimal(result);
  s21_decimal temp_res = {0};
  s21_decimal temp = value_1;

  for (int i = 0; i < 96; i++) {
    if (s21_get_bit(value_2, i) == 1) {
      if (s21_add(temp_res, temp, &temp_res) != 0) {
        while (scale > 0) {
          if (s21_bank_round(&temp_res, scale - 1)) {
            return 1;
          }
          scale--;
          if (!(temp_res.bits[2] & 0x80000000)) {
            break;
          }
        }
        if (!(temp_res.bits[2] & 0x80000000)) {
          return 2;
        }
      }
    }
    if (s21_shift_left(&temp) != 0) {
      continue;
    }
  }
  if (scale > 28) {
    if (s21_bank_round(&temp_res, 28) != 0) {
      return 1;
    }
    scale = 28;
  } else {
    s21_set_scale(&temp_res, scale);
  }
  *result = temp_res;
  s21_set_sign(result, result_sign);

  return 0;
}
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return 1;
  }
  s21_zero_decimal(result);

  if (s21_is_zero(value_2)) {
    return 3;
  }
  if (s21_is_zero(value_1)) {
    return 0;
  }

  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int result_sign = (sign1 != sign2) ? 1 : 0;

  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);

  s21_set_sign(&value_1, 0);
  s21_set_sign(&value_2, 0);

  // Выравниваем масштабы перед делением
  int scale = scale1 - scale2;

  while (scale < 0) {
    if (s21_mul_mantissa_by_10(&value_1)) {
      return 1;
    }
    scale++;
  }

  while (scale2 < scale1) {
    if (s21_mul_mantissa_by_10(&value_2)) {
      return 1;
    }
    scale2++;
  }

  int result_scale = 0;

  s21_decimal result_temp = {0};
  s21_decimal remainder = {0};

  // Деление в столбик
  for (int i = 95; i >= 0; i--) {
    s21_shift_left(&remainder);
    int bit = s21_get_bit(value_1, i);
    if (bit) {
      s21_set_bit(&remainder, 0, 1);
    }
    if (s21_compare_mantissa(remainder, value_2) >= 0) {
      s21_sub_mantissa(remainder, value_2, &remainder);
      s21_set_bit(&result_temp, i, 1);
    }
  }

  // Формирование дробной части из остатка
  if (!s21_is_zero(remainder)) {
    s21_decimal fractional = {0};
    s21_decimal temp_remainder = remainder;
    int digits = 0;
    int max_digits = 28;

    while (!s21_is_zero(temp_remainder) && digits < max_digits) {
      s21_mul_mantissa_by_10(&temp_remainder);

      int digit = 0;
      s21_decimal temp_copy = temp_remainder;
      while (s21_compare_mantissa(temp_copy, value_2) >= 0) {
        s21_sub_mantissa(temp_copy, value_2, &temp_copy);
        digit++;
      }
      temp_remainder = temp_copy;

      s21_mul_mantissa_by_10(&fractional);
      s21_decimal digit_dec = {0};
      digit_dec.bits[0] = digit;
      s21_add_mantissa(fractional, digit_dec, &fractional);

      digits++;
      result_scale++;
    }

    // Собираем результат: result_temp * 10^digits + fractional
    for (int i = 0; i < digits; i++) {
      s21_mul_mantissa_by_10(&result_temp);
    }
    s21_add_mantissa(result_temp, fractional, &result_temp);
  }

  if (result_scale > 28) {
    if (s21_bank_round(&result_temp, 28) != 0) {
      return 1;
    }
    result_scale = 28;
  }

  s21_set_scale(&result_temp, result_scale);

  *result = result_temp;
  s21_set_sign(result, result_sign);

  return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    if (dst == NULL) return 1;
    s21_zero_decimal(dst);                  // обнуляем все биты
    if (src == 0) return 0;

    unsigned int val = src < 0 ? -src : src;
    dst->bits[0] = val;          
    if (src < 0) s21_set_sign(dst, 1);      // установить знак
    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    if (dst == NULL || !s21_is_valid(src)) return 1;
    if (s21_is_zero(src)) {
      *dst = 0;
      return 0;
    }

    int scale = s21_get_scale(src);

    // Отбрасываем дробную часть (целочисленное деление на 10^scale)
    while (scale-- > 0) {
      s21_div_mantissa_by_10(&src);
    }

    if (src.bits[2] != 0 || src.bits[1] != 0) return 1; // число больше int

    unsigned int val = src.bits[0];
    int sign = s21_get_sign(src);
    if (sign) {
      if(val > 2147483648U) return 1;
      *dst = -(int)val;
    } else {
      if(val > 2147483647U) return 1;
      *dst = (int)val;
    }
    return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (dst == NULL) return 1;
  s21_zero_decimal(dst);                  // обнуляем все биты
  if (src == 0.0f) return 0;

  float f = src;
  int sign = 0;
  if (f < 0) { sign = 1; f = -f; }
  const float max_dec = 7.9228162514264337593543950335e28f; // 2^96 - 1
  if ((src > 0 && src < 1e-28) || f > max_dec) return 1;

  if (src < 0) s21_set_sign(dst, 1);

  char buf[64];
  snprintf(buf, sizeof(buf), "%.10e", f);

  char *e_pos = strchr(buf, 'e');
  if (!e_pos) return 1;

  char mant_buf[64];
  int mant_len = 0;
  for (char *p = buf; p < e_pos; p++) {
    if (*p >= '0' && *p <= '9') mant_buf[mant_len++] = *p;
  }
  mant_buf[mant_len] = '\0';
  int exp = atoi(e_pos + 1);

  // if (mant_len == 0 || strspn(mant_buf, "0") == mant_len) return 0;

  if (mant_len > 7) {
    int carry = 0;
    int eighth = mant_buf[7] - '0';
    if (eighth > 5) {
      carry = 1;
    } else if (eighth == 5) {
      int seventh = mant_buf[6] - '0';
      if (seventh % 2 == 1) carry = 1;
    }
    char new_mant[8];
    strncpy(new_mant, mant_buf, 7);
    new_mant[7] = '\0';
    if (carry) {
      int idx = 6;
      while (idx >= 0 && new_mant[idx] == '9') {
        new_mant[idx] = '0';
        idx--;
      }
      if (idx >= 0) {
        new_mant[idx] += 1;
      } else {
        new_mant[0] = '1';
        for (int i = 1; i < 7; i++) new_mant[i] = '0';
        exp += 1;
      }
    }
    strcpy(mant_buf, new_mant);
    mant_len = 7;
  }

  s21_decimal M;
  s21_zero_decimal(&M);
  for (int i = 0; i < mant_len; i++) {
    if (s21_mul_mantissa_by_10(&M)) return 1;
    uint32_t digit = mant_buf[i] - '0';
    s21_decimal digit_dec;
    s21_zero_decimal(&digit_dec);
    digit_dec.bits[0] = digit;
    if (s21_add_mantissa(M, digit_dec, &M)) return 1;
  }

  int E = exp - (mant_len - 1);
  int scale = 0;
  s21_decimal N = M;

  if (E >= 0) {
    for (int i = 0; i < E; i++) {
      if (s21_mul_mantissa_by_10(&N)) return 1;
    }
    scale = 0;
  } else {
    scale = -E;
  }

  while (scale > 0) {
    s21_decimal temp = N;
    int rem = s21_div_mantissa_by_10(&temp);
    if (rem == 0) {
        s21_div_mantissa_by_10(&N);
        scale--;
    } else {
        break;
    }
  }
  
  if (scale > 28) return 1;

  dst->bits[0] = N.bits[0];
  dst->bits[1] = N.bits[1];
  dst->bits[2] = N.bits[2];
  s21_set_scale(dst, scale);
  s21_set_sign(dst, sign);

  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL || !s21_is_valid(src)) return 1;
  if (s21_is_zero(src)) {
    *dst = 0.0f;
    return 0;
  }

  long double mant = 0.0L;
  mant = (long double)src.bits[2];
  mant = mant * 4294967296.0L + (long double)src.bits[1];
  mant = mant * 4294967296.0L + (long double)src.bits[0];
  
  int scale = s21_get_scale(src);
  long double divisor = 1.0L;
  for (int i = 0; i < scale; i++) divisor *= 10.0L;
  mant /= divisor;
  
  if (s21_get_sign(src)) mant = -mant;

  *dst = (float)mant;
  return 0;
}