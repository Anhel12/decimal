#include "s21_decimal.h"
#include "s21_helpers.h"
#include <stdio.h>
#include <math.h>

void print_test_result(const char *test_name, int expected, int actual) {
  printf("%s: ", test_name);
  if (expected == actual) {
    printf("\033[32mОК\033[0m (ожидалось: %d, получено: %d)\n", expected,
           actual);
  } else {
    printf("\033[31mFAIL\033[0m  (ожидалось: %d, получено: %d)\n", expected,
           actual);
  }
}

void print_header(const char *title) {
  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║  %-52s ║\n", title);
  printf("╚══════════════════════════════════════════════════════════╝\n");
}

void print_subheader(const char *title) { printf("\n▶ %s\n", title); }

int main() {
  s21_decimal d = {0};
  int status = 0;
  int total_tests = 0;
  int passed_tests = 0;

  // ============================================================
  // БЛОК 1: s21_get_bit и s21_set_bit
  // ============================================================
  print_header("ТЕСТЫ s21_get_bit и s21_set_bit");
  print_subheader("Тесты установки/сброса битов (мантисса 0-95)");

  s21_zero_decimal(&d);
  s21_set_bit(&d, 0, 1);
  total_tests++;
  if (s21_get_bit(d, 0) == 1)
    passed_tests++;
  print_test_result("Установка бита 0 в 1", 1, s21_get_bit(d, 0));

  s21_set_bit(&d, 0, 0);
  total_tests++;
  if (s21_get_bit(d, 0) == 0)
    passed_tests++;
  print_test_result("Сброс бита 0 в 0", 0, s21_get_bit(d, 0));

  s21_set_bit(&d, 31, 1);
  total_tests++;
  if (s21_get_bit(d, 31) == 1)
    passed_tests++;
  print_test_result("Установка бита 31 в 1 (последний в bits[0])", 1,
                    s21_get_bit(d, 31));

  s21_set_bit(&d, 32, 1);
  total_tests++;
  if (s21_get_bit(d, 32) == 1)
    passed_tests++;
  print_test_result("Установка бита 32 в 1 (первый в bits[1])", 1,
                    s21_get_bit(d, 32));

  s21_set_bit(&d, 63, 1);
  total_tests++;
  if (s21_get_bit(d, 63) == 1)
    passed_tests++;
  print_test_result("Установка бита 63 в 1 (последний в bits[1])", 1,
                    s21_get_bit(d, 63));

  s21_set_bit(&d, 64, 1);
  total_tests++;
  if (s21_get_bit(d, 64) == 1)
    passed_tests++;
  print_test_result("Установка бита 64 в 1 (первый в bits[2])", 1,
                    s21_get_bit(d, 64));

  s21_set_bit(&d, 95, 1);
  total_tests++;
  if (s21_get_bit(d, 95) == 1)
    passed_tests++;
  print_test_result("Установка бита 95 в 1 (последний в мантиссе)", 1,
                    s21_get_bit(d, 95));

  total_tests++;
  if (s21_get_bit(d, 1) == 0)
    passed_tests++;
  print_test_result("Бит 1 остался 0", 0, s21_get_bit(d, 1));

  total_tests++;
  if (s21_get_bit(d, 33) == 0)
    passed_tests++;
  print_test_result("Бит 33 остался 0", 0, s21_get_bit(d, 33));

  s21_set_bit(&d, 96, 1);
  total_tests++;
  if (s21_get_bit(d, 96) == 0)
    passed_tests++;
  print_test_result("Попытка установить бит 96 (должен игнорироваться)", 0,
                    s21_get_bit(d, 96));

  s21_set_bit(&d, 100, 1);
  total_tests++;
  if (s21_get_bit(d, 100) == 0)
    passed_tests++;
  print_test_result("Попытка установить бит 100 (должен игнорироваться)", 0,
                    s21_get_bit(d, 100));

  print_subheader("Тесты чтения битов из разных элементов");

  s21_zero_decimal(&d);
  d.bits[0] = 0b1010;
  total_tests++;
  if (s21_get_bit(d, 0) == 0)
    passed_tests++;
  print_test_result("Бит 0 из bits[0]=0b1010", 0, s21_get_bit(d, 0));
  total_tests++;
  if (s21_get_bit(d, 1) == 1)
    passed_tests++;
  print_test_result("Бит 1 из bits[0]=0b1010", 1, s21_get_bit(d, 1));
  total_tests++;
  if (s21_get_bit(d, 2) == 0)
    passed_tests++;
  print_test_result("Бит 2 из bits[0]=0b1010", 0, s21_get_bit(d, 2));
  total_tests++;
  if (s21_get_bit(d, 3) == 1)
    passed_tests++;
  print_test_result("Бит 3 из bits[0]=0b1010", 1, s21_get_bit(d, 3));

  s21_zero_decimal(&d);
  d.bits[1] = 0b1010;
  total_tests++;
  if (s21_get_bit(d, 32) == 0)
    passed_tests++;
  print_test_result("Бит 32 из bits[1]=0b1010", 0, s21_get_bit(d, 32));
  total_tests++;
  if (s21_get_bit(d, 33) == 1)
    passed_tests++;
  print_test_result("Бит 33 из bits[1]=0b1010", 1, s21_get_bit(d, 33));
  total_tests++;
  if (s21_get_bit(d, 34) == 0)
    passed_tests++;
  print_test_result("Бит 34 из bits[1]=0b1010", 0, s21_get_bit(d, 34));
  total_tests++;
  if (s21_get_bit(d, 35) == 1)
    passed_tests++;
  print_test_result("Бит 35 из bits[1]=0b1010", 1, s21_get_bit(d, 35));

  s21_zero_decimal(&d);
  d.bits[2] = 0b1010;
  total_tests++;
  if (s21_get_bit(d, 64) == 0)
    passed_tests++;
  print_test_result("Бит 64 из bits[2]=0b1010", 0, s21_get_bit(d, 64));
  total_tests++;
  if (s21_get_bit(d, 65) == 1)
    passed_tests++;
  print_test_result("Бит 65 из bits[2]=0b1010", 1, s21_get_bit(d, 65));
  total_tests++;
  if (s21_get_bit(d, 66) == 0)
    passed_tests++;
  print_test_result("Бит 66 из bits[2]=0b1010", 0, s21_get_bit(d, 66));
  total_tests++;
  if (s21_get_bit(d, 67) == 1)
    passed_tests++;
  print_test_result("Бит 67 из bits[2]=0b1010", 1, s21_get_bit(d, 67));

  print_subheader("Тесты граничных битов мантиссы");

  s21_zero_decimal(&d);
  d.bits[0] = 0x80000000;
  d.bits[1] = 0x80000001;
  d.bits[2] = 0x80000001;
  total_tests++;
  if (s21_get_bit(d, 31) == 1)
    passed_tests++;
  print_test_result("Бит 31 (последний в bits[0])", 1, s21_get_bit(d, 31));
  total_tests++;
  if (s21_get_bit(d, 32) == 1)
    passed_tests++;
  print_test_result("Бит 32 (первый в bits[1])", 1, s21_get_bit(d, 32));
  total_tests++;
  if (s21_get_bit(d, 63) == 1)
    passed_tests++;
  print_test_result("Бит 63 (последний в bits[1])", 1, s21_get_bit(d, 63));
  total_tests++;
  if (s21_get_bit(d, 64) == 1)
    passed_tests++;
  print_test_result("Бит 64 (первый в bits[2])", 1, s21_get_bit(d, 64));
  total_tests++;
  if (s21_get_bit(d, 95) == 1)
    passed_tests++;
  print_test_result("Бит 95 (последний в мантиссе)", 1, s21_get_bit(d, 95));

  print_subheader("Тесты невалидных индексов");

  s21_zero_decimal(&d);
  d.bits[0] = 5;
  total_tests++;
  if (s21_get_bit(d, -1) == 0)
    passed_tests++;
  print_test_result("Бит -1 (невалидный)", 0, s21_get_bit(d, -1));
  total_tests++;
  if (s21_get_bit(d, 96) == 0)
    passed_tests++;
  print_test_result("Бит 96 (невалидный)", 0, s21_get_bit(d, 96));
  total_tests++;
  if (s21_get_bit(d, 100) == 0)
    passed_tests++;
  print_test_result("Бит 100 (невалидный)", 0, s21_get_bit(d, 100));
  total_tests++;
  if (s21_get_bit(d, 150) == 0)
    passed_tests++;
  print_test_result("Бит 150 (невалидный)", 0, s21_get_bit(d, 150));

  // ============================================================
  // БЛОК 2: s21_zero_decimal
  // ============================================================
  print_header("ТЕСТЫ s21_zero_decimal");
  print_subheader("Тесты обнуления");

  s21_zero_decimal(&d);
  s21_set_bit(&d, 0, 1);
  s21_set_bit(&d, 31, 1);
  s21_set_bit(&d, 64, 1);
  d.bits[3] = 0x80020000;

  s21_zero_decimal(&d);
  total_tests++;
  if (d.bits[0] == 0)
    passed_tests++;
  print_test_result("bits[0] обнулён", 0, d.bits[0]);
  total_tests++;
  if (d.bits[1] == 0)
    passed_tests++;
  print_test_result("bits[1] обнулён", 0, d.bits[1]);
  total_tests++;
  if (d.bits[2] == 0)
    passed_tests++;
  print_test_result("bits[2] обнулён", 0, d.bits[2]);
  total_tests++;
  if (d.bits[3] == 0)
    passed_tests++;
  print_test_result("bits[3] обнулён", 0, d.bits[3]);

  s21_zero_decimal(NULL);

  // ============================================================
  // БЛОК 3: s21_get_sign и s21_set_sign
  // ============================================================
  print_header("ТЕСТЫ s21_get_sign и s21_set_sign");
  print_subheader("Тесты знака (через отдельные методы)");

  s21_zero_decimal(&d);
  total_tests++;
  if (s21_get_sign(d) == 0)
    passed_tests++;
  print_test_result("Знак по умолчанию", 0, s21_get_sign(d));

  s21_set_sign(&d, 1);
  total_tests++;
  if (s21_get_sign(d) == 1)
    passed_tests++;
  print_test_result("Установка знака в 1", 1, s21_get_sign(d));

  s21_set_sign(&d, 0);
  total_tests++;
  if (s21_get_sign(d) == 0)
    passed_tests++;
  print_test_result("Установка знака в 0", 0, s21_get_sign(d));

  s21_set_sign(&d, 5);
  total_tests++;
  if (s21_get_sign(d) == 0)
    passed_tests++;
  print_test_result("Попытка установить sign=5 (должен игнорироваться)", 0,
                    s21_get_sign(d));

  s21_set_sign(&d, -1);
  total_tests++;
  if (s21_get_sign(d) == 0)
    passed_tests++;
  print_test_result("Попытка установить sign=-1 (должен игнорироваться)", 0,
                    s21_get_sign(d));

  // ============================================================
  // БЛОК 4: s21_get_scale и s21_set_scale
  // ============================================================
  print_header("ТЕСТЫ s21_get_scale и s21_set_scale");
  print_subheader("Тесты точности (scale)");

  s21_zero_decimal(&d);
  total_tests++;
  if (s21_get_scale(d) == 0)
    passed_tests++;
  print_test_result("Scale по умолчанию", 0, s21_get_scale(d));

  s21_set_scale(&d, 5);
  total_tests++;
  if (s21_get_scale(d) == 5)
    passed_tests++;
  print_test_result("Установка scale=5", 5, s21_get_scale(d));

  s21_set_scale(&d, 28);
  total_tests++;
  if (s21_get_scale(d) == 28)
    passed_tests++;
  print_test_result("Установка scale=28 (максимум)", 28, s21_get_scale(d));

  s21_set_scale(&d, 0);
  total_tests++;
  if (s21_get_scale(d) == 0)
    passed_tests++;
  print_test_result("Установка scale=0", 0, s21_get_scale(d));

  s21_set_scale(&d, 30);
  total_tests++;
  if (s21_get_scale(d) == 0)
    passed_tests++;
  print_test_result("Попытка установить scale=30 (должен игнорироваться)", 0,
                    s21_get_scale(d));

  s21_set_scale(&d, -5);
  total_tests++;
  if (s21_get_scale(d) == 0)
    passed_tests++;
  print_test_result("Попытка установить scale=-5 (должен игнорироваться)", 0,
                    s21_get_scale(d));

  print_subheader("Тесты совместной работы sign и scale");

  s21_zero_decimal(&d);
  s21_set_sign(&d, 1);
  s21_set_scale(&d, 10);
  total_tests++;
  if (s21_get_sign(d) == 1 && s21_get_scale(d) == 10)
    passed_tests++;
  print_test_result("Sign=1 и scale=10 установлены корректно", 1, 1);

  s21_set_scale(&d, 15);
  total_tests++;
  if (s21_get_scale(d) == 15 && s21_get_sign(d) == 1)
    passed_tests++;
  print_test_result("Scale изменён на 15, sign сохранился", 1, 1);

  s21_set_sign(&d, 0);
  total_tests++;
  if (s21_get_sign(d) == 0 && s21_get_scale(d) == 15)
    passed_tests++;
  print_test_result("Sign изменён на 0, scale сохранился", 1, 1);

  // ============================================================
  // БЛОК 5: Итоговые тесты установки чисел
  // ============================================================
  print_header("ИТОГОВЫЕ ТЕСТЫ - ПОЛНАЯ УСТАНОВКА ЧИСЕЛ");
  print_subheader("Число 123.45 (положительное)");

  s21_zero_decimal(&d);
  d.bits[0] = 12345;
  s21_set_scale(&d, 2);
  s21_set_sign(&d, 0);

  total_tests++;
  if (d.bits[0] == 12345)
    passed_tests++;
  print_test_result("Мантисса = 12345", 12345, d.bits[0]);
  total_tests++;
  if (s21_get_scale(d) == 2)
    passed_tests++;
  print_test_result("Scale = 2", 2, s21_get_scale(d));
  total_tests++;
  if (s21_get_sign(d) == 0)
    passed_tests++;
  print_test_result("Sign = 0 (положительное)", 0, s21_get_sign(d));

  print_subheader("Число -0.007 (отрицательное)");

  s21_zero_decimal(&d);
  d.bits[0] = 7;
  s21_set_scale(&d, 3);
  s21_set_sign(&d, 1);

  total_tests++;
  if (d.bits[0] == 7)
    passed_tests++;
  print_test_result("Мантисса = 7", 7, d.bits[0]);
  total_tests++;
  if (s21_get_scale(d) == 3)
    passed_tests++;
  print_test_result("Scale = 3", 3, s21_get_scale(d));
  total_tests++;
  if (s21_get_sign(d) == 1)
    passed_tests++;
  print_test_result("Sign = 1 (отрицательное)", 1, s21_get_sign(d));

  // ============================================================
  // БЛОК 6: s21_is_zero
  // ============================================================
  print_header("ТЕСТЫ s21_is_zero");

  s21_zero_decimal(&d);
  total_tests++;
  if (s21_is_zero(d) == 1)
    passed_tests++;
  print_test_result("Ноль (все биты = 0)", 1, s21_is_zero(d));

  s21_zero_decimal(&d);
  d.bits[0] = 1;
  total_tests++;
  if (s21_is_zero(d) == 0)
    passed_tests++;
  print_test_result("Не ноль (bits[0] = 1)", 0, s21_is_zero(d));

  s21_zero_decimal(&d);
  d.bits[1] = 1;
  total_tests++;
  if (s21_is_zero(d) == 0)
    passed_tests++;
  print_test_result("Не ноль (bits[1] = 1)", 0, s21_is_zero(d));

  s21_zero_decimal(&d);
  d.bits[2] = 1;
  total_tests++;
  if (s21_is_zero(d) == 0)
    passed_tests++;
  print_test_result("Не ноль (bits[2] = 1)", 0, s21_is_zero(d));

  s21_zero_decimal(&d);
  d.bits[3] = 0x80020000;
  total_tests++;
  if (s21_is_zero(d) == 1)
    passed_tests++;
  print_test_result("Ноль с sign=1 и scale=2", 1, s21_is_zero(d));

  // ============================================================
  // БЛОК 7: s21_is_valid
  // ============================================================
  print_header("ТЕСТЫ s21_is_valid");

  s21_zero_decimal(&d);
  s21_set_scale(&d, 5);
  s21_set_sign(&d, 1);
  total_tests++;
  if (s21_is_valid(d) == 1)
    passed_tests++;
  print_test_result("Корректный decimal (scale=5, sign=1)", 1, s21_is_valid(d));

  s21_zero_decimal(&d);
  s21_set_scale(&d, 28);
  total_tests++;
  if (s21_is_valid(d) == 1)
    passed_tests++;
  print_test_result("scale=28 (максимум)", 1, s21_is_valid(d));

  s21_zero_decimal(&d);
  d.bits[3] = (30 << 16);
  total_tests++;
  if (s21_is_valid(d) == 0)
    passed_tests++;
  print_test_result("scale=30 (должен быть 0-28)", 0, s21_is_valid(d));

  s21_zero_decimal(&d);
  d.bits[3] = 0x00008000;
  total_tests++;
  if (s21_is_valid(d) == 0)
    passed_tests++;
  print_test_result("Бит 15 в bits[3] = 1 (должен быть 0)", 0, s21_is_valid(d));

  s21_zero_decimal(&d);
  d.bits[3] = 0x01000000;
  total_tests++;
  if (s21_is_valid(d) == 0)
    passed_tests++;
  print_test_result("Бит 24 в bits[3] = 1 (должен быть 0)", 0, s21_is_valid(d));

  s21_zero_decimal(&d);
  d.bits[3] = 0x40000000;
  total_tests++;
  if (s21_is_valid(d) == 0)
    passed_tests++;
  print_test_result("Бит 30 в bits[3] = 1 (должен быть 0)", 0, s21_is_valid(d));

  s21_zero_decimal(&d);
  d.bits[3] = 0x801C0000;
  total_tests++;
  if (s21_is_valid(d) == 1)
    passed_tests++;
  print_test_result("sign=1, scale=28 (корректно)", 1, s21_is_valid(d));

  s21_zero_decimal(&d);
  total_tests++;
  if (s21_is_valid(d) == 1)
    passed_tests++;
  print_test_result("Нулевой decimal (корректный)", 1, s21_is_valid(d));

  // ============================================================
  // БЛОК 8: s21_compare_mantissa
  // ============================================================
  print_header("ТЕСТЫ s21_compare_mantissa");

  s21_decimal a = {0}, b = {0};

  a.bits[2] = 5;
  b.bits[2] = 3;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 1)
    passed_tests++;
  print_test_result("a > b (bits[2]: 5 > 3)", 1, s21_compare_mantissa(a, b));

  a.bits[2] = 3;
  b.bits[2] = 5;
  total_tests++;
  if (s21_compare_mantissa(a, b) == -1)
    passed_tests++;
  print_test_result("a < b (bits[2]: 3 < 5)", -1, s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 7;
  b.bits[1] = 3;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 1)
    passed_tests++;
  print_test_result("a > b (bits[1]: 7 > 3)", 1, s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 3;
  b.bits[1] = 7;
  total_tests++;
  if (s21_compare_mantissa(a, b) == -1)
    passed_tests++;
  print_test_result("a < b (bits[1]: 3 < 7)", -1, s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 3;
  b.bits[1] = 3;
  a.bits[0] = 1;
  b.bits[0] = 1;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 0)
    passed_tests++;
  print_test_result("a == b (все равны)", 0, s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 3;
  b.bits[1] = 3;
  a.bits[0] = 7;
  b.bits[0] = 3;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 1)
    passed_tests++;
  print_test_result("a > b (bits[0]: 7 > 3)", 1, s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 3;
  b.bits[1] = 3;
  a.bits[0] = 3;
  b.bits[0] = 7;
  total_tests++;
  if (s21_compare_mantissa(a, b) == -1)
    passed_tests++;
  print_test_result("a < b (bits[0]: 3 < 7)", -1, s21_compare_mantissa(a, b));

  s21_zero_decimal(&a);
  s21_zero_decimal(&b);
  total_tests++;
  if (s21_compare_mantissa(a, b) == 0)
    passed_tests++;
  print_test_result("a == b (нули)", 0, s21_compare_mantissa(a, b));

  a.bits[2] = 0xFFFFFFFF;
  b.bits[2] = 0xFFFFFFFE;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 1)
    passed_tests++;
  print_test_result("a > b (0xFFFFFFFF > 0xFFFFFFFE)", 1,
                    s21_compare_mantissa(a, b));

  a.bits[2] = 5;
  b.bits[2] = 5;
  a.bits[1] = 0xFFFFFFFF;
  b.bits[1] = 0x00000000;
  total_tests++;
  if (s21_compare_mantissa(a, b) == 1)
    passed_tests++;
  print_test_result("a > b (bits[1]: 0xFFFFFFFF > 0x00000000)", 1,
                    s21_compare_mantissa(a, b));

  // ============================================================
  // БЛОК 9: s21_normalize
  // ============================================================
  print_header("ТЕСТЫ s21_normalize");

  s21_decimal d1 = {0}, d2 = {0};
  int result_norm = 0;

  s21_zero_decimal(&d1);
  s21_zero_decimal(&d2);
  d1.bits[0] = 123;
  d2.bits[0] = 456;
  s21_set_scale(&d1, 2);
  s21_set_scale(&d2, 2);
  s21_normalize(&d1, &d2);
  total_tests++;
  int test1_passed = (s21_get_scale(d1) == 2 && s21_get_scale(d2) == 2 &&
                      d1.bits[0] == 123 && d2.bits[0] == 456);
  if (test1_passed)
    passed_tests++;
  print_test_result("Нормализация одинаковых scale (2 и 2)", 1, test1_passed);

  s21_zero_decimal(&d1);
  s21_zero_decimal(&d2);
  d1.bits[0] = 123;
  d2.bits[0] = 456;
  s21_set_scale(&d1, 2);
  s21_set_scale(&d2, 3);
  s21_normalize(&d1, &d2);
  total_tests++;
  int test2_passed = (s21_get_scale(d1) == 3 && s21_get_scale(d2) == 3 &&
                      d1.bits[0] == 1230 && d2.bits[0] == 456);
  if (test2_passed)
    passed_tests++;
  print_test_result("Нормализация scale (2 и 3) → (3 и 3)", 1, test2_passed);

  s21_zero_decimal(&d1);
  s21_zero_decimal(&d2);
  d1.bits[0] = 5;
  d2.bits[0] = 123;
  s21_set_scale(&d1, 0);
  s21_set_scale(&d2, 2);
  s21_normalize(&d1, &d2);
  total_tests++;
  int test3_passed = (s21_get_scale(d1) == 2 && s21_get_scale(d2) == 2 &&
                      d1.bits[0] == 500 && d2.bits[0] == 123);
  if (test3_passed)
    passed_tests++;
  print_test_result("Нормализация scale (0 и 2) → (2 и 2)", 1, test3_passed);

  s21_zero_decimal(&d1);
  s21_zero_decimal(&d2);
  d1.bits[0] = 12345;
  d2.bits[0] = 67890;
  s21_set_scale(&d1, 2);
  s21_set_scale(&d2, 4);
  s21_normalize(&d1, &d2);
  total_tests++;
  int test4_passed = (d2.bits[0] == 67890 && s21_get_scale(d2) == 4);
  if (test4_passed)
    passed_tests++;
  print_test_result("Мантисса d2 не изменилась (67890)", 1, test4_passed);

  total_tests++;
  int test5_passed = (d1.bits[0] == 1234500 && s21_get_scale(d1) == 4);
  if (test5_passed)
    passed_tests++;
  print_test_result("Мантисса d1 стала 1234500", 1, test5_passed);

  total_tests++;
  int test6_passed =
      (s21_normalize(NULL, &d2) == 1 && s21_normalize(&d1, NULL) == 1);
  if (test6_passed)
    passed_tests++;
  print_test_result("NULL указатели обработаны корректно", 1, test6_passed);

  s21_zero_decimal(&d1);
  s21_zero_decimal(&d2);
  d1.bits[0] = 0xFFFFFFFF;
  d1.bits[1] = 0xFFFFFFFF;
  d1.bits[2] = 0xFFFFFFFF;
  s21_set_scale(&d1, 0);
  d2.bits[0] = 1;
  s21_set_scale(&d2, 28);
  result_norm = s21_normalize(&d1, &d2);
  total_tests++;
  if (result_norm == 1)
    passed_tests++;
  print_test_result("Переполнение при нормализации", 1, result_norm);

  // ============================================================
  // БЛОК 10: s21_add
  // ============================================================
  print_header("ТЕСТЫ s21_add");

  s21_decimal num1 = {0}, num2 = {0}, result_add = {0};

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 1;
  num2.bits[0] = 2;
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 3 && status == 0)
    passed_tests++;
  print_test_result("1 + 2 = 3", 3, result_add.bits[0]);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num2.bits[0] = 5;
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 5 && status == 0)
    passed_tests++;
  print_test_result("0 + 5 = 5", 5, result_add.bits[0]);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 5;
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 5 && status == 0)
    passed_tests++;
  print_test_result("5 + 0 = 5", 5, result_add.bits[0]);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 0 && status == 0)
    passed_tests++;
  print_test_result("0 + 0 = 0", 0, result_add.bits[0]);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 123;
  num2.bits[0] = 456;
  s21_set_scale(&num1, 2);
  s21_set_scale(&num2, 2);
  status = s21_add(num1, num2, &result_add);
  int add_test5_passed = (result_add.bits[0] == 579 &&
                          s21_get_scale(result_add) == 2 && status == 0);
  total_tests++;
  if (add_test5_passed)
    passed_tests++;
  print_test_result("1.23 + 4.56 = 5.79 (мантисса=579, scale=2)", 1,
                    add_test5_passed);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 5;
  num2.bits[0] = 7;
  s21_set_scale(&num2, 3);
  status = s21_add(num1, num2, &result_add);
  int add_test6_passed = (result_add.bits[0] == 5007 &&
                          s21_get_scale(result_add) == 3 && status == 0);
  total_tests++;
  if (add_test6_passed)
    passed_tests++;
  print_test_result("5 + 0.007 = 5.007 (мантисса=5007, scale=3)", 1,
                    add_test6_passed);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 1;
  num2.bits[0] = 1;
  s21_set_sign(&num1, 1);
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 0 && s21_get_sign(result_add) == 0 && status == 0)
    passed_tests++;
  print_test_result("-1 + 1 = 0", 0, result_add.bits[0]);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 5;
  num2.bits[0] = 3;
  s21_set_sign(&num1, 1);
  s21_set_sign(&num2, 1);
  status = s21_add(num1, num2, &result_add);
  total_tests++;
  if (result_add.bits[0] == 8 && s21_get_sign(result_add) == 1 && status == 0)
    passed_tests++;
  print_test_result("-5 + (-3) = -8", -8,
                    result_add.bits[0] * (s21_get_sign(result_add) ? -1 : 1));

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 123;
  num2.bits[0] = 45;
  s21_set_scale(&num1, 2);
  s21_set_scale(&num2, 2);
  s21_set_sign(&num2, 1);
  status = s21_add(num1, num2, &result_add);
  int add_test9_passed =
      (result_add.bits[0] == 78 && s21_get_scale(result_add) == 2 &&
       s21_get_sign(result_add) == 0 && status == 0);
  total_tests++;
  if (add_test9_passed)
    passed_tests++;
  print_test_result("1.23 + (-0.45) = 0.78 (мантисса=78, scale=2)", 1,
                    add_test9_passed);

  status = s21_add(num1, num2, NULL);
  total_tests++;
  if (status == 1)
    passed_tests++;
  print_test_result("NULL указатель", 1, status);

  s21_zero_decimal(&num1);
  s21_zero_decimal(&num2);
  s21_zero_decimal(&result_add);
  num1.bits[0] = 5123;
  num2.bits[0] = 2987;
  s21_set_scale(&num1, 3);
  s21_set_scale(&num2, 3);
  status = s21_add(num1, num2, &result_add);
  int add_test11_passed = (result_add.bits[0] == 8110 &&
                           s21_get_scale(result_add) == 3 && status == 0);
  total_tests++;
  if (add_test11_passed)
    passed_tests++;
  print_test_result("5.123 + 2.987 = 8.110 (мантисса=8110, scale=3)", 1,
                    add_test11_passed);

  // ============================================================
  // БЛОК 11: s21_sub
  // ============================================================
  print_header("ТЕСТЫ s21_sub");

  s21_decimal num3 = {0}, num4 = {0}, result_sub = {0};

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 5;
  num4.bits[0] = 3;
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 2 && s21_get_sign(result_sub) == 0 && status == 0)
    passed_tests++;
  print_test_result("5 - 3 = 2", 2, result_sub.bits[0]);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 3;
  num4.bits[0] = 5;
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 2 && s21_get_sign(result_sub) == 1 && status == 0)
    passed_tests++;
  print_test_result("3 - 5 = -2", -2,
                    result_sub.bits[0] * (s21_get_sign(result_sub) ? -1 : 1));

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num4.bits[0] = 5;
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 5 && s21_get_sign(result_sub) == 1 && status == 0)
    passed_tests++;
  print_test_result("0 - 5 = -5", -5,
                    result_sub.bits[0] * (s21_get_sign(result_sub) ? -1 : 1));

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 5;
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 5 && s21_get_sign(result_sub) == 0 && status == 0)
    passed_tests++;
  print_test_result("5 - 0 = 5", 5, result_sub.bits[0]);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 0 && s21_get_sign(result_sub) == 0 && status == 0)
    passed_tests++;
  print_test_result("0 - 0 = 0", 0, result_sub.bits[0]);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 523;
  num4.bits[0] = 211;
  s21_set_scale(&num3, 2);
  s21_set_scale(&num4, 2);
  status = s21_sub(num3, num4, &result_sub);
  int sub_test6_passed = (result_sub.bits[0] == 312 &&
                          s21_get_scale(result_sub) == 2 && status == 0);
  total_tests++;
  if (sub_test6_passed)
    passed_tests++;
  print_test_result("5.23 - 2.11 = 3.12 (мантисса=312, scale=2)", 1,
                    sub_test6_passed);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 211;
  num4.bits[0] = 523;
  s21_set_scale(&num3, 2);
  s21_set_scale(&num4, 2);
  status = s21_sub(num3, num4, &result_sub);
  int sub_test7_passed =
      (result_sub.bits[0] == 312 && s21_get_scale(result_sub) == 2 &&
       s21_get_sign(result_sub) == 1 && status == 0);
  total_tests++;
  if (sub_test7_passed)
    passed_tests++;
  print_test_result("2.11 - 5.23 = -3.12 (мантисса=312, scale=2)", 1,
                    sub_test7_passed);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 5;
  num4.bits[0] = 3;
  s21_set_sign(&num3, 1);
  s21_set_sign(&num4, 1);
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 2 && s21_get_sign(result_sub) == 1 && status == 0)
    passed_tests++;
  print_test_result("-5 - (-3) = -2", -2,
                    result_sub.bits[0] * (s21_get_sign(result_sub) ? -1 : 1));

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 3;
  num4.bits[0] = 5;
  s21_set_sign(&num3, 1);
  s21_set_sign(&num4, 1);
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 2 && s21_get_sign(result_sub) == 0 && status == 0)
    passed_tests++;
  print_test_result("-3 - (-5) = 2", 2, result_sub.bits[0]);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 5;
  num4.bits[0] = 3;
  s21_set_sign(&num4, 1);
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 8 && s21_get_sign(result_sub) == 0 && status == 0)
    passed_tests++;
  print_test_result("5 - (-3) = 8", 8, result_sub.bits[0]);

  s21_zero_decimal(&num3);
  s21_zero_decimal(&num4);
  s21_zero_decimal(&result_sub);
  num3.bits[0] = 5;
  num4.bits[0] = 3;
  s21_set_sign(&num3, 1);
  status = s21_sub(num3, num4, &result_sub);
  total_tests++;
  if (result_sub.bits[0] == 8 && s21_get_sign(result_sub) == 1 && status == 0)
    passed_tests++;
  print_test_result("-5 - 3 = -8", -8,
                    result_sub.bits[0] * (s21_get_sign(result_sub) ? -1 : 1));

  status = s21_sub(num3, num4, NULL);
  total_tests++;
  if (status == 1)
    passed_tests++;
  print_test_result("NULL указатель", 1, status);

  // ============================================================
  // БЛОК 12: s21_mul
  // ============================================================
  print_header("ТЕСТЫ s21_mul");

  s21_decimal num5 = {0}, num6 = {0}, result_mul = {0};

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 2;
  num6.bits[0] = 3;
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 6 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("2 * 3 = 6", 6, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 5;
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 0 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("5 * 0 = 0", 0, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num6.bits[0] = 5;
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 0 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("0 * 5 = 0", 0, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 0 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("0 * 0 = 0", 0, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 2;
  num6.bits[0] = 3;
  s21_set_sign(&num5, 1);
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 6 && s21_get_sign(result_mul) == 1 && status == 0)
    passed_tests++;
  print_test_result("-2 * 3 = -6", -6,
                    result_mul.bits[0] * (s21_get_sign(result_mul) ? -1 : 1));

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 2;
  num6.bits[0] = 3;
  s21_set_sign(&num6, 1);
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 6 && s21_get_sign(result_mul) == 1 && status == 0)
    passed_tests++;
  print_test_result("2 * (-3) = -6", -6,
                    result_mul.bits[0] * (s21_get_sign(result_mul) ? -1 : 1));

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 2;
  num6.bits[0] = 3;
  s21_set_sign(&num5, 1);
  s21_set_sign(&num6, 1);
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 6 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("-2 * (-3) = 6", 6, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 12;
  num6.bits[0] = 34;
  s21_set_scale(&num5, 1);
  s21_set_scale(&num6, 1);
  status = s21_mul(num5, num6, &result_mul);
  int mul_test8_passed = (result_mul.bits[0] == 408 &&
                          s21_get_scale(result_mul) == 2 && status == 0);
  total_tests++;
  if (mul_test8_passed)
    passed_tests++;
  print_test_result("1.2 * 3.4 = 4.08 (мантисса=408, scale=2)", 1,
                    mul_test8_passed);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 25;
  num6.bits[0] = 4;
  s21_set_scale(&num5, 1);
  status = s21_mul(num5, num6, &result_mul);
  int mul_test9_passed = (result_mul.bits[0] == 100 &&
                          s21_get_scale(result_mul) == 1 && status == 0);
  total_tests++;
  if (mul_test9_passed)
    passed_tests++;
  print_test_result("2.5 * 4 = 10.0 (мантисса=100, scale=1)", 1,
                    mul_test9_passed);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 10;
  num6.bits[0] = 10;
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 100 && s21_get_sign(result_mul) == 0 && status == 0)
    passed_tests++;
  print_test_result("10 * 10 = 100", 100, result_mul.bits[0]);

  s21_zero_decimal(&num5);
  s21_zero_decimal(&num6);
  s21_zero_decimal(&result_mul);
  num5.bits[0] = 999;
  num6.bits[0] = 999;
  status = s21_mul(num5, num6, &result_mul);
  total_tests++;
  if (result_mul.bits[0] == 998001 && status == 0)
    passed_tests++;
  print_test_result("999 * 999 = 998001", 998001, result_mul.bits[0]);

  status = s21_mul(num5, num6, NULL);
  total_tests++;
  if (status == 1)
    passed_tests++;
  print_test_result("NULL указатель", 1, status);

  // ============================================================
  // БЛОК 13: s21_div
  // ============================================================
  print_header("ТЕСТЫ s21_div");

  s21_decimal num7 = {0}, num8 = {0}, result_div = {0};

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 6;
  num8.bits[0] = 3;
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 2 && s21_get_sign(result_div) == 0 && status == 0)
    passed_tests++;
  print_test_result("6 / 3 = 2", 2, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 10;
  num8.bits[0] = 2;
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 5 && s21_get_sign(result_div) == 0 && status == 0)
    passed_tests++;
  print_test_result("10 / 2 = 5", 5, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 5;
  num8.bits[0] = 2;
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 25 && s21_get_sign(result_div) == 0 && status == 0)
    passed_tests++;
  print_test_result("5 / 2 = 2 (целая часть)", 25, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num8.bits[0] = 5;
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 0 && s21_get_sign(result_div) == 0 && status == 0)
    passed_tests++;
  print_test_result("0 / 5 = 0", 0, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 5;
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (status == 3)
    passed_tests++;
  print_test_result("5 / 0 = ошибка (код 3)", 3, status);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 6;
  num8.bits[0] = 3;
  s21_set_sign(&num7, 1);
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 2 && s21_get_sign(result_div) == 1 && status == 0)
    passed_tests++;
  print_test_result("-6 / 3 = -2", -2,
                    result_div.bits[0] * (s21_get_sign(result_div) ? -1 : 1));

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 6;
  num8.bits[0] = 3;
  s21_set_sign(&num8, 1);
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 2 && s21_get_sign(result_div) == 1 && status == 0)
    passed_tests++;
  print_test_result("6 / (-3) = -2", -2,
                    result_div.bits[0] * (s21_get_sign(result_div) ? -1 : 1));

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 6;
  num8.bits[0] = 3;
  s21_set_sign(&num7, 1);
  s21_set_sign(&num8, 1);
  status = s21_div(num7, num8, &result_div);
  total_tests++;
  if (result_div.bits[0] == 2 && s21_get_sign(result_div) == 0 && status == 0)
    passed_tests++;
  print_test_result("-6 / (-3) = 2", 2, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 12;
  num8.bits[0] = 4;
  s21_set_scale(&num7, 1);
  s21_set_scale(&num8, 1);
  status = s21_div(num7, num8, &result_div);
  int div_test9_passed = (result_div.bits[0] == 3 &&
                          s21_get_scale(result_div) == 0 && status == 0);
  total_tests++;
  if (div_test9_passed)
    passed_tests++;
  print_test_result("1.2 / 0.4 = 3", 3, result_div.bits[0]);

  s21_zero_decimal(&num7);
  s21_zero_decimal(&num8);
  s21_zero_decimal(&result_div);
  num7.bits[0] = 25;
  num8.bits[0] = 2;
  s21_set_scale(&num7, 1);
  status = s21_div(num7, num8, &result_div);
  int div_test10_passed = (result_div.bits[0] == 125 &&
                           s21_get_scale(result_div) == 2 && status == 0);
  total_tests++;
  if (div_test10_passed)
    passed_tests++;
  print_test_result("2.5 / 2 = 1.25 (мантисса=125, scale=2)", 1,
                    div_test10_passed);

  status = s21_div(num7, num8, NULL);
  total_tests++;
  if (status == 1)
    passed_tests++;
  print_test_result("NULL указатель", 1, status);

  // ============================================================
  // БЛОК: s21_from_int_to_decimal
  // ============================================================
  print_header("ТЕСТЫ s21_from_int_to_decimal");

  int res;
  int success;

  print_subheader("Проверка некорректного указателя");

  total_tests++;
  res = s21_from_int_to_decimal(123, NULL);
  if (res == 1) passed_tests++;
  print_test_result("Передача NULL указателя (возврат 1)", 1, res);

  print_subheader("Преобразование нуля");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(0, &d);
  success = (res == 0 && d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("0 -> все биты нулевые, знак 0", 1, success);

  print_subheader("Преобразование положительных чисел");

  // 1
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(1, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("1 -> bits[0]=1, знак 0", 1, success);

  // 123
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(123, &d);
  success = (res == 0 && d.bits[0] == 123 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("123 -> bits[0]=123, знак 0", 1, success);

  // 2147483647 (максимальный int)
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(2147483647, &d);
  success = (res == 0 && d.bits[0] == 2147483647u && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("2147483647 -> bits[0]=2147483647, знак 0", 1, success);

  print_subheader("Преобразование отрицательных чисел");

  // -1
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(-1, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0x80000000);
  if (success) passed_tests++;
  print_test_result("-1 -> bits[0]=1, знак 1", 1, success);

  // -123
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(-123, &d);
  success = (res == 0 && d.bits[0] == 123 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0x80000000);
  if (success) passed_tests++;
  print_test_result("-123 -> bits[0]=123, знак 1", 1, success);

  // -2147483648 (минимальный int)
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_int_to_decimal(-2147483648, &d);
  success = (res == 0 && d.bits[0] == 2147483648u && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0x80000000);
  if (success) passed_tests++;
  print_test_result("-2147483648 -> bits[0]=2147483648, знак 1", 1, success);

  print_subheader("Проверка отсутствия побочных эффектов");

  // Проверяем, что bits[1] и bits[2] остаются нулевыми для разных чисел
  s21_zero_decimal(&d);
  s21_from_int_to_decimal(42, &d);
  total_tests++;
  success = (d.bits[1] == 0 && d.bits[2] == 0);
  if (success) passed_tests++;
  print_test_result("bits[1] и bits[2] равны 0 для 42", 1, success);

  s21_zero_decimal(&d);
  s21_from_int_to_decimal(-777, &d);
  total_tests++;
  success = (d.bits[1] == 0 && d.bits[2] == 0);
  if (success) passed_tests++;
  print_test_result("bits[1] и bits[2] равны 0 для -777", 1, success);

  // Проверка масштаба (биты 16-23) – всегда 0
  s21_zero_decimal(&d);
  s21_from_int_to_decimal(999, &d);
  total_tests++;
  success = ((d.bits[3] & 0x00FF0000) == 0);
  if (success) passed_tests++;
  print_test_result("Масштаб (биты 16-23) равен 0 для 999", 1, success);

  s21_zero_decimal(&d);
  s21_from_int_to_decimal(-999, &d);
  total_tests++;
  success = ((d.bits[3] & 0x00FF0000) == 0);
  if (success) passed_tests++;
  print_test_result("Масштаб (биты 16-23) равен 0 для -999", 1, success);

  // ============================================================
  // БЛОК: s21_from_decimal_to_int
  // ============================================================
  print_header("ТЕСТЫ s21_from_decimal_to_int");

  int result;
  int dst;

  print_subheader("Проверка некорректных указателей и невалидных decimal");

  // dst == NULL
  total_tests++;
  result = s21_from_decimal_to_int(d, NULL);
  if (result == 1) passed_tests++;
  print_test_result("Передача NULL dst (возврат 1)", 1, result);

  // Невалидный decimal (биты 24-31 установлены)
  s21_zero_decimal(&d);
  d.bits[3] = 0xFFFFFFFF;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  if (result == 1) passed_tests++;
  print_test_result("Невалидный decimal (bits[3] с запрещёнными битами) -> возврат 1", 1, result);

  // Масштаб > 28
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  d.bits[3] = 29 << 16;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  if (result == 1) passed_tests++;
  print_test_result("Невалидный decimal (масштаб 29) -> возврат 1", 1, result);

  print_subheader("Преобразование нуля");

  s21_zero_decimal(&d);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("Ноль -> dst = 0, возврат 0", 1, success);

  print_subheader("Преобразование положительных целых чисел (scale = 0)");

  // 1
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 1);
  if (success) passed_tests++;
  print_test_result("1 -> dst = 1", 1, success);

  // 123
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 123);
  if (success) passed_tests++;
  print_test_result("123 -> dst = 123", 1, success);

  // 2147483647
  s21_zero_decimal(&d);
  d.bits[0] = 2147483647u;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 2147483647);
  if (success) passed_tests++;
  print_test_result("2147483647 -> dst = 2147483647", 1, success);

  print_subheader("Преобразование отрицательных целых чисел (scale = 0)");

  // -1
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == -1);
  if (success) passed_tests++;
  print_test_result("-1 -> dst = -1", 1, success);

  // -123
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == -123);
  if (success) passed_tests++;
  print_test_result("-123 -> dst = -123", 1, success);

  // -2147483648
  s21_zero_decimal(&d);
  d.bits[0] = 2147483648u;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == -2147483648);
  if (success) passed_tests++;
  print_test_result("-2147483648 -> dst = -2147483648", 1, success);

  print_subheader("Преобразование чисел с дробной частью (отбрасывание)");

  // 123.45 (scale=2)
  s21_zero_decimal(&d);
  d.bits[0] = 12345;
  s21_set_scale(&d, 2);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 123);
  if (success) passed_tests++;
  print_test_result("123.45 -> dst = 123", 1, success);

  // 0.999 (scale=3)
  s21_zero_decimal(&d);
  d.bits[0] = 999;
  d.bits[3] = 3 << 16;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("0.999 -> dst = 0", 1, success);

  // -123.45 (scale=2)
  s21_zero_decimal(&d);
  d.bits[0] = 12345;
  d.bits[3] = 2 << 16;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == -123);
  if (success) passed_tests++;
  print_test_result("-123.45 -> dst = -123", 1, success);

  // -0.999 (scale=3)
  s21_zero_decimal(&d);
  d.bits[0] = 999;
  d.bits[3] = 3 << 16;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("-0.999 -> dst = 0", 1, success);

  // 0.5 (scale=1)
  s21_zero_decimal(&d);
  d.bits[0] = 5;
  d.bits[3] = 1 << 16;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("0.5 -> dst = 0", 1, success);

  // -0.5 (scale=1)
  s21_zero_decimal(&d);
  d.bits[0] = 5;
  d.bits[3] = 1 << 16;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("-0.5 -> dst = 0", 1, success);

  print_subheader("Переполнение (число выходит за пределы int)");

  // 2147483648 (max int + 1)
  s21_zero_decimal(&d);
  d.bits[0] = 2147483648u;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 1 && dst == 0);
  if (success) passed_tests++;
  print_test_result("2147483648 -> возврат 1 (переполнение)", 1, success);

  // -2147483649
  s21_zero_decimal(&d);
  d.bits[0] = 2147483649u;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 1 && dst == 0);
  if (success) passed_tests++;
  print_test_result("-2147483649 -> возврат 1", 1, success);

  // bits[1] != 0
  s21_zero_decimal(&d);
  d.bits[1] = 1;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 1 && dst == 0);
  if (success) passed_tests++;
  print_test_result("2^32 (bits[1]=1) -> возврат 1", 1, success);

  // bits[2] != 0
  s21_zero_decimal(&d);
  d.bits[2] = 1;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 1 && dst == 0);
  if (success) passed_tests++;
  print_test_result("2^64 (bits[2]=1) -> возврат 1", 1, success);

  // После деления на 10 всё равно больше int
  s21_zero_decimal(&d);
  d.bits[1] = 5;      // 5 * 2^32 = 21474836480
  d.bits[0] = 0;
  d.bits[3] = 1 << 16; // scale=1, после деления = 2147483648
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 1 && dst == 0);
  if (success) passed_tests++;
  print_test_result("5*2^32 с масштабом 1 -> возврат 1", 1, success);

  print_subheader("Краевые случаи с масштабом и делением");

  // 0.05 -> 0
  s21_zero_decimal(&d);
  d.bits[0] = 5;
  d.bits[3] = 2 << 16;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("0.05 -> dst = 0", 1, success);

  // Максимальный масштаб 28, число маленькое -> 0
  s21_zero_decimal(&d);
  d.bits[0] = 123456789;
  d.bits[3] = 28 << 16;
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 0);
  if (success) passed_tests++;
  print_test_result("123456789 с масштабом 28 -> dst = 0", 1, success);

  // Успешное преобразование с масштабом, после деления получается int
  s21_zero_decimal(&d);
  d.bits[0] = 1234567890;
  d.bits[3] = 1 << 16; // 123456789
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == 123456789);
  if (success) passed_tests++;
  print_test_result("1234567890 с масштабом 1 -> dst = 123456789", 1, success);

  // Отрицательное с масштабом
  s21_zero_decimal(&d);
  d.bits[0] = 1234567890;
  d.bits[3] = 1 << 16;
  s21_set_sign(&d, 1);
  total_tests++;
  result = s21_from_decimal_to_int(d, &dst);
  success = (result == 0 && dst == -123456789);
  if (success) passed_tests++;
  print_test_result("-1234567890 с масштабом 1 -> dst = -123456789", 1, success);

  // ============================================================
  // БЛОК: s21_from_float_to_decimal
  // ============================================================
  print_header("ТЕСТЫ s21_from_float_to_decimal");

  print_subheader("Проверка некорректного указателя");

  total_tests++;
  res = s21_from_float_to_decimal(1.0f, NULL);
  if (res == 1) passed_tests++;
  print_test_result("Передача NULL указателя (возврат 1)", 1, res);

  print_subheader("Преобразование нуля");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(0.0f, &d);
  success = (res == 0 && d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("0.0 -> нулевой decimal", 1, success);

  print_subheader("Преобразование целых чисел");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(1.0f, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 0 && s21_get_sign(d) == 0);
  if (success) passed_tests++;
  print_test_result("1.0 -> bits[0]=1, scale=0, sign=0", 1, success);

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(-1.0f, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 0 && s21_get_sign(d) == 1);
  if (success) passed_tests++;
  print_test_result("-1.0 -> bits[0]=1, scale=0, sign=1", 1, success);

  print_subheader("Преобразование чисел с дробной частью");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(0.1f, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 1 && s21_get_sign(d) == 0);
  if (success) passed_tests++;
  print_test_result("0.1 -> bits[0]=1, scale=1, sign=0", 1, success);

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(-0.1f, &d);
  success = (res == 0 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 1 && s21_get_sign(d) == 1);
  if (success) passed_tests++;
  print_test_result("-0.1 -> bits[0]=1, scale=1, sign=1", 1, success);

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(123.456f, &d);
  success = (res == 0 && d.bits[0] == 123456 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 3 && s21_get_sign(d) == 0);
  if (success) passed_tests++;
  print_test_result("123.456 -> bits[0]=123456, scale=3, sign=0", 1, success);

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(-123.456f, &d);
  success = (res == 0 && d.bits[0] == 123456 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 3 && s21_get_sign(d) == 1);
  if (success) passed_tests++;
  print_test_result("-123.456 -> bits[0]=123456, scale=3, sign=1", 1, success);

  print_subheader("Проверка округления до 7 значащих цифр");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(1.23456789f, &d);
  success = (res == 0 && d.bits[0] == 1234568 && d.bits[1] == 0 && d.bits[2] == 0 && s21_get_scale(d) == 6 && s21_get_sign(d) == 0);
  if (success) passed_tests++;
  print_test_result("1.23456789 -> 1.234568 (scale=6, mantissa=1234568)", 1, success);

  print_subheader("Проверка граничных значений");

  // слишком маленькое (меньше 1e-28)
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(1e-30f, &d);
  if (res == 1) passed_tests++;
  print_test_result("1e-30 -> ошибка (возврат 1)", 1, res);

  // слишком большое (больше max)
  s21_zero_decimal(&d);
  total_tests++;
  float big_val = 1e30f; // 1e30 > 7.9e28, должно вернуть ошибку
  res = s21_from_float_to_decimal(big_val, &d);
  if (res == 1) passed_tests++;
  print_test_result("1e30 -> ошибка (возврат 1)", 1, res);

  // бесконечность
  s21_zero_decimal(&d);
  float inf = 1.0f / 0.0f;
  total_tests++;
  res = s21_from_float_to_decimal(inf, &d);
  if (res == 1) passed_tests++;
  print_test_result("inf -> ошибка", 1, res);

  // -inf
  s21_zero_decimal(&d);
  float neg_inf = -1.0f / 0.0f;
  total_tests++;
  res = s21_from_float_to_decimal(neg_inf, &d);
  if (res == 1) passed_tests++;
  print_test_result("-inf -> ошибка", 1, res);

  // NaN
  s21_zero_decimal(&d);
  float nan_val = 0.0f / 0.0f;
  total_tests++;
  res = s21_from_float_to_decimal(nan_val, &d);
  if (res == 1) passed_tests++;
  print_test_result("NaN -> ошибка", 1, res);

  print_subheader("Проверка корректности масштаба (не больше 28)");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(1e-28f, &d);
  success = (res == 0 && s21_get_scale(d) == 28 && d.bits[0] == 1 && d.bits[1] == 0 && d.bits[2] == 0);
  if (success) passed_tests++;
  print_test_result("1e-28 -> scale=28, bits[0]=1", 1, success);

  // Проверка, что при ошибке dst остаётся нулевым
  s21_zero_decimal(&d);
  float big_val2 = 1e30f;
  total_tests++;
  res = s21_from_float_to_decimal(big_val2, &d);
  success = (res == 1 && d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("При ошибке dst остаётся нулевым", 1, success);

  // -0.0 -> должен быть 0 без знака
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(-0.0f, &d);
  success = (res == 0 && d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0 && d.bits[3] == 0);
  if (success) passed_tests++;
  print_test_result("-0.0 -> нулевой decimal (знак не устанавливается)", 1, success);

  // Проверка, что знак устанавливается для отрицательных чисел
  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_float_to_decimal(-123.456f, &d);
  success = (res == 0 && s21_get_sign(d) == 1);
  if (success) passed_tests++;
  print_test_result("-123.456 -> sign=1", 1, success);

  // ============================================================
  // БЛОК: s21_from_decimal_to_float
  // ============================================================
  print_header("ТЕСТЫ s21_from_decimal_to_float");

  float dst_float;
  float expected;

  // Для сравнения float используем эпсилон
  #define EPS 1e-6f

  print_subheader("Проверка некорректных указателей и невалидных decimal");

  // dst == NULL
  total_tests++;
  res = s21_from_decimal_to_float(d, NULL);
  if (res == 1) passed_tests++;
  print_test_result("Передача NULL dst (возврат 1)", 1, res);

  // Невалидный decimal (биты 24-31 установлены)
  s21_zero_decimal(&d);
  d.bits[3] = 0xFFFFFFFF;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  if (res == 1) passed_tests++;
  print_test_result("Невалидный decimal (bits[3] с запрещёнными битами) -> возврат 1", 1, res);

  // Масштаб > 28
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  d.bits[3] = 29 << 16;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  if (res == 1) passed_tests++;
  print_test_result("Невалидный decimal (масштаб 29) -> возврат 1", 1, res);

  print_subheader("Преобразование нуля");

  s21_zero_decimal(&d);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  if (res == 0 && dst_float == 0.0f) passed_tests++;
  print_test_result("Ноль -> dst = 0.0, возврат 0", 1, (res == 0 && dst_float == 0.0f));

  print_subheader("Преобразование целых чисел (scale = 0)");

  // 1
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 1.0f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("1 -> dst ≈ 1.0", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // -1
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  d.bits[3] |= (1u << 31);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = -1.0f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("-1 -> dst ≈ -1.0", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // 123
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 123.0f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("123 -> dst ≈ 123.0", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // -123
  s21_zero_decimal(&d);
  d.bits[0] = 123;
  d.bits[3] |= (1u << 31);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = -123.0f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("-123 -> dst ≈ -123.0", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // 2147483647 (максимальное положительное int, помещается в float)
  s21_zero_decimal(&d);
  d.bits[0] = 2147483647u;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 2147483647.0f; // float потеряет точность, но приблизительно
  if (res == 0 && fabsf(dst_float - expected) / expected < 1e-7f) passed_tests++;
  print_test_result("2147483647 -> dst ≈ 2.147e9", 1, (res == 0 && fabsf(dst_float - expected) / expected < 1e-7f));

  print_subheader("Преобразование чисел с дробной частью (scale > 0)");

  // 0.1 (мантисса 1, scale=1)
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  d.bits[3] = 1 << 16; // scale=1
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 0.1f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("0.1 -> dst ≈ 0.1", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // -0.1
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  d.bits[3] = (1 << 16) | (1u << 31);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = -0.1f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("-0.1 -> dst ≈ -0.1", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // 123.456 (мантисса 123456, scale=3)
  s21_zero_decimal(&d);
  d.bits[0] = 123456;
  d.bits[3] = 3 << 16;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 123.456f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("123.456 -> dst ≈ 123.456", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // -123.456
  s21_zero_decimal(&d);
  d.bits[0] = 123456;
  d.bits[3] = (3 << 16) | (1u << 31);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = -123.456f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("-123.456 -> dst ≈ -123.456", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  // 0.000001 (мантисса 1, scale=6)
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  d.bits[3] = 6 << 16;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 1e-6f;
  if (res == 0 && fabsf(dst_float - expected) < EPS) passed_tests++;
  print_test_result("0.000001 -> dst ≈ 1e-6", 1, (res == 0 && fabsf(dst_float - expected) < EPS));

  print_subheader("Граничные значения");

  // Максимальное положительное число, которое помещается в float (мантисса 79228162514264337593543950335, scale=0)
  // Но это число больше максимального float (~3.4e38), поэтому мы не можем его представить точно.
  // Проверим, что функция не возвращает ошибку и даёт какое-то значение.
  // Однако в задании не сказано проверять переполнение, просто преобразование.
  // Поэтому используем допустимое значение: 1e28 (scale=0) или с масштабом.
  // Для простоты возьмём 1e28, которое точно помещается в float как 1e28f.
  s21_zero_decimal(&d);
  // 1e28 = 10000000000000000000000000000 (28 нулей) - это 29 цифр, но мы можем задать bits[1] и bits[0]
  // Проще использовать уже готовое число 1e28, но его сложно закодировать в bits.
  // Вместо этого возьмём число, которое гарантированно влезает в float: 1234567890.123456 (масштаб 6)
  // или просто 1234567890 (масштаб 0) - помещается в float без потери точности?
  // 1234567890f - float точно представляет только до 7 значащих цифр, но сравним с погрешностью.
  s21_zero_decimal(&d);
  d.bits[0] = 1234567890u;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 1234567890.0f;
  if (res == 0 && fabsf(dst_float - expected) / expected < 1e-7f) passed_tests++;
  print_test_result("1234567890 (scale=0) -> dst ≈ 1.23456789e9", 1, (res == 0 && fabsf(dst_float - expected) / expected < 1e-7f));

  // Минимальное отрицательное: -1234567890
  s21_zero_decimal(&d);
  d.bits[0] = 1234567890u;
  d.bits[3] |= (1u << 31);
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = -1234567890.0f;
  if (res == 0 && fabsf(dst_float - expected) / fabsf(expected) < 1e-7f) passed_tests++;
  print_test_result("-1234567890 (scale=0) -> dst ≈ -1.23456789e9", 1, (res == 0 && fabsf(dst_float - expected) / fabsf(expected) < 1e-7f));

  // Максимально возможный масштаб 28 и маленькая мантисса 1 -> 1e-28
  s21_zero_decimal(&d);
  d.bits[0] = 1;
  d.bits[3] = 28 << 16;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  expected = 1e-28f; // Это число меньше минимального нормализованного float (~1.18e-38), но 1e-28 > 1e-38, так что представимо как денормализованное
  // Ожидаем, что функция вернёт 0 и установит значение, которое может быть не точно 1e-28, но приблизительно.
  if (res == 0 && fabsf(dst_float - expected) < 1e-27f) passed_tests++;
  print_test_result("1e-28 (scale=28) -> dst ≈ 1e-28", 1, (res == 0 && fabsf(dst_float - expected) < 1e-27f));

  print_subheader("Проверка, что при ошибке dst не изменяется");

  s21_zero_decimal(&d);
  d.bits[3] = 0xFFFFFFFF; // невалидный
  float backup = 123.456f;
  dst_float = backup;
  total_tests++;
  res = s21_from_decimal_to_float(d, &dst_float);
  if (res == 1 && dst_float == backup) passed_tests++;
  print_test_result("При ошибке dst остаётся неизменным", 1, (res == 1 && dst_float == backup));

  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║  РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ                                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("  \033[32m✅ Пройдено:\033[0m %d тестов\n", passed_tests);
  printf("  \033[31m❌ Провалено:\033[0m  %d тестов\n",
         total_tests - passed_tests);
  printf("  📊 Успешность: %.1f%%\n", (float)passed_tests / total_tests * 100);
  printf("  📝 Всего тестов: %d\n", total_tests);

  return 0;
}
