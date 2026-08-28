CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Основные файлы
SRCS = s21_helpers.c s21_decimal.c
OBJS = $(SRCS:.c=.o)
LIB_NAME = s21_decimal.a

all: $(LIB_NAME)

# Основная библиотека
$(LIB_NAME): $(OBJS)
	ar rcs $@ $^
	rm -f $(OBJS)

# Компиляция объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Тесты (использует библиотеку)
tests: $(LIB_NAME)
	$(CC) $(CFLAGS) main.c -L. -l:s21_decimal.a  -o tests -lm
	./tests

# GCOV отчёт
gcov_report:
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_helpers.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_decimal.c
	ar rcs $(LIB_NAME) s21_helpers.o s21_decimal.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) main.c $(LIB_NAME) -lgcov -o tests_gcov
	./tests_gcov
	lcov -t "s21_decimal" -o s21_decimal.info -c -d .
	genhtml -o gcov_report s21_decimal.info
	rm -f *.o tests_gcov *.gcda *.gcno *.info

# Очистка
clean:
	rm -f $(LIB_NAME) *.o tests tests_gcov *.gcda *.gcno *.gcov *.info
	rm -rf gcov_report

.PHONY: all clean tests gcov_report