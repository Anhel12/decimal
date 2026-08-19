#include "s21_helpers.h"

#include <stdint.h>
#include <stdio.h>

// метод для получения конкретного бита
int s21_get_bit(s21_decimal d, int index) {
  if ((index < 0 || index > 95) && index != 127) {
    return 0;
  }
  return ((uint32_t)d.bits[index / 32] >> index % 32) & 1;
}
// метод для установки бита
void s21_set_bit(s21_decimal *d, int index, int value) {
  if (d == NULL) {
    return;
  }
  if ((index < 0 || index > 95) && index != 127) {
    return;
  }
  uint32_t temp = (uint32_t)d->bits[index / 32];
  if (value) {
    temp = temp | (1 << index % 32);
  } else {
    temp = temp & ~(1 << index % 32);
  }
  d->bits[index / 32] = (int)temp;
}
// метод для обнуления битов
void s21_zero_decimal(s21_decimal *d) {
  if (d == NULL) {
    return;
  }
  for (int i = 0; i < 4; i++) {
    d->bits[i] = 0;
  }
}
// метод для получения знака
int s21_get_sign(s21_decimal d) { return s21_get_bit(d, 127); }

// метод для установки знака
void s21_set_sign(s21_decimal *d, int sign) {
  if (d == NULL) {
    return;
  }
  if (sign <= 1 && sign >= 0) {
    s21_set_bit(d, 127, sign);
  }
}
// метод для получения точности
int s21_get_scale(s21_decimal d) { return (d.bits[3] >> 16) & 0xFF; }
// метод для установки точности
void s21_set_scale(s21_decimal *d, int scale) {
  if (d == NULL) {
    return;
  }
  if (scale <= 28 && scale >= 0) {
    d->bits[3] = d->bits[3] & ~(0xFF << 16);
    d->bits[3] = d->bits[3] | (scale << 16);
  }
}
// проверка равно ли число нулю
int s21_is_zero(s21_decimal d) {
  if (d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0) {
    return 1;
  }
  return 0;
}
// проверка является ли число корректным
int s21_is_valid(s21_decimal d) {
  // биты с 0-15 24-30 должны быть равны 0
  // 0x7F000000  0-15
  // 0x0000FFFF  24-30
  if (s21_get_scale(d) > 28 || (d.bits[3] & 0x7F00FFFF)) {
    return 0;
  }
  return 1;
}
// сравнивает мантиссы
int s21_compare_mantissa(s21_decimal a, s21_decimal b) {
  for (int i = 2; i >= 0; i--) {
    uint32_t a_bits = (uint32_t)a.bits[i];
    uint32_t b_bits = (uint32_t)b.bits[i];
    if (a_bits > b_bits) {
      return 1;
    } else if (a_bits < b_bits) {
      return -1;
    }
  }
  return 0;
}
// домножение мантиссы на 10
int s21_mul_mantissa_by_10(s21_decimal *d) {
  if (d == NULL) {
    return 1;
  }

  int carry = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t temp = (uint64_t)(uint32_t)d->bits[i] * 10 + carry;
    d->bits[i] = (int)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }
  if (carry != 0) {
    return 1;
  }
  return 0;
}
// делим мантиссу на 10
int s21_div_mantissa_by_10(s21_decimal *d) {
  if (d == NULL) {
    return 1;
  }
  uint64_t carry = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t temp = (uint64_t)carry << 32 | (uint32_t)d->bits[i];
    d->bits[i] = (uint32_t)(temp / 10);
    carry = temp % 10;
  }
  return (int)carry;
}
// добавляем единицу к мантиссе
int s21_add_mantissa_one(s21_decimal *d) {
  if (d == NULL) {
    return 1;
  }

  uint64_t temp = (uint64_t)(uint32_t)d->bits[0] + 1;
  d->bits[0] = (uint32_t)(temp & 0xFFFFFFFF);
  uint64_t carry = temp >> 32;

  if (carry) {
    temp = (uint64_t)(uint32_t)d->bits[1] + carry;
    d->bits[1] = (uint32_t)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }

  if (carry) {
    temp = (uint64_t)(uint32_t)d->bits[2] + carry;
    d->bits[2] = (uint32_t)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }

  if (carry) {
    return 1; // переполнение
  }

  return 0; // успех
}
// приведение к одной точности
int s21_normalize(s21_decimal *d1, s21_decimal *d2) {
  if (d1 == NULL || d2 == NULL) {
    return 1;
  }
  if (s21_get_scale(*d1) == s21_get_scale(*d2)) {
    return 0;
  }
  if (s21_get_scale(*d1) > s21_get_scale(*d2)) {
    for (int i = 0; i < s21_get_scale(*d1) - s21_get_scale(*d2); i++) {
      int error = s21_mul_mantissa_by_10(d2);
      if (error) {
        return 1;
      }
    }

    s21_set_scale(d2, s21_get_scale(*d1));
  } else {
    for (int i = 0; i < s21_get_scale(*d2) - s21_get_scale(*d1); i++) {
      int error = s21_mul_mantissa_by_10(d1);
      if (error) {
        return 1;
      }
    }
    s21_set_scale(d1, s21_get_scale(*d2));
  }
  return 0;
}

// банковское окргуление, даже близко не то же самое что и round! Валера если ты
// читаешь это, не получится просто переименовать метод они делают разное!!!
int s21_bank_round(s21_decimal *d, int target_scale) {
  if (d == NULL) {
    return 1;
  }
  int scale = s21_get_scale(*d);
  if (scale <= target_scale) {
    return 0;
  }
  if (scale > 28 || scale < 0) {
    return 1;
  }
  int last_digit = 0;
  int prev_last = 0;
  for (int i = 0; i < scale - target_scale; i++) {
    prev_last = last_digit;
    last_digit = s21_div_mantissa_by_10(d);
  }
  if (last_digit > 5 || (last_digit == 5 && prev_last % 2 != 0)) {
    s21_add_mantissa_one(d);
  }
  s21_set_scale(d, target_scale);
  return 0;
}
// сдвиг бита влево с переносом
int s21_shift_left(s21_decimal *d) {
  if (d == NULL)
    return 1;

  uint32_t carry = 0;
  uint32_t next_carry = 0;

  for (int i = 0; i < 3; i++) {
    next_carry = (d->bits[i] >> 31) & 1; // старший бит текущего слова
    d->bits[i] = (d->bits[i] << 1) | carry; // сдвиг с переносом
    carry = next_carry;
  }

  // Если есть перенос из старшего слова → переполнение
  if (carry != 0) {
    return 1;
  }

  return 0;
}
// быстрый метод для вычитания мантисс
// не использовал тот что есть потому что этому плевать на точность и знаки
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result) {
  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t temp =
        (uint64_t)(uint32_t)a.bits[i] - (uint64_t)(uint32_t)b.bits[i] - borrow;
    result->bits[i] = (uint32_t)(temp & 0xFFFFFFFF);
    borrow = (temp >> 32) & 1;
  }
  return borrow ? 1 : 0;
}
// быстрый метод сложения мантисс
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result) {
  uint64_t carry = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t temp =
        (uint64_t)(uint32_t)a.bits[i] + (uint64_t)(uint32_t)b.bits[i] + carry;
    result->bits[i] = (uint32_t)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }
  return carry ? 1 : 0;
}