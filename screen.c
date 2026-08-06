#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int get_int_length(int value) {
  int length = 0;
  int copy_value = value;
  if (value == 0) {
    return 1;
  }
  if (copy_value < 0) {
    length++;
    copy_value = -copy_value;
  }
  while (copy_value > 0) {
    copy_value /= 10;
    length++;
  }
  return length;
}

size_t get_double_length(double value) {
  char buffer[512];

  int len = snprintf(buffer, sizeof(buffer), "%g", value);

  if (len < 0)
    return 0;
  return (size_t)len;
}

size_t get_float_length(float value) {
  char buffer[64];
  int len = snprintf(buffer, sizeof(buffer), "%g", value);

  if (len < 0)
    return 0;
  return (size_t)len;
}

size_t get_bool_length(bool value) { return (value) ? 4 : 5; }

size_t get_string_length(const char *value) { return strlen(value); }

size_t get_value_size(DataValue value, DataType type) {
  switch (type) {
  case TYPE_INT:
    return get_int_length(value.int_val);
  case TYPE_FLOAT:
    return get_float_length(value.float_val);
  case TYPE_DOUBLE:
    return get_double_length(value.double_val);
  case TYPE_STRING:
    return get_string_length(value.string_val);
  case TYPE_BOOL:
    return get_bool_length(value.bool_val);
  }
}

void print_value(DataValue value, DataType type, size_t padding) {
  switch (type) {
  case TYPE_INT:
    printf("%-*d", (int)padding, value.int_val);
    break;
  case TYPE_FLOAT:
    printf("%-*f", (int)padding, value.float_val);
    break;
  case TYPE_DOUBLE:
    printf("%-*lf", (int)padding, value.double_val);
    break;
  case TYPE_STRING:
    printf("%-*s", (int)padding, value.string_val);
    //    for (size_t i = 0; i < strlen(value.string_val); ++i) {
    //      putchar(value.string_val[i]);
    //    }
    break;
  case TYPE_BOOL:
    printf("%-*s", (int)padding, value.bool_val ? "true" : "false");
    break;
  }
}

void clear_screen() { printf("\033[H\033[J"); };

void get_screen_width(size_t *rows, size_t *cols) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  *rows = w.ws_row;
  *cols = w.ws_col;
}

// вся колонка с данными + название возвращает длину всей таблицы
size_t get_cols_size(Table *table, size_t *result) {
  size_t max_len_column = 0;
  size_t curren_cell_size = 0;
  size_t lenght_table = 0;
  for (size_t i = 0; i < table->columns_count; ++i) {
    max_len_column = table->columns[i].name_length_column;
    for (size_t j = 0; j < table->row_count; ++j) {
      curren_cell_size =
          get_value_size(table->rows[j].values[i], table->columns[i].type);
      if (max_len_column < curren_cell_size) {
        max_len_column = curren_cell_size;
      }
    }
    lenght_table += max_len_column;
    result[i] = max_len_column;
  }
  return lenght_table + (table->columns_count * 2) - 1;
}

void print_table(Table *table) {
  clear_screen();
  size_t rows = 0;
  size_t cols = 0;
  size_t table_width = 0;
  size_t table_height = 0;
  size_t *cols_size = malloc(sizeof(size_t) * table->columns_count);

  get_screen_width(&rows, &cols);
  table_width = get_cols_size(table, cols_size);
  table_height = table->row_count;

  for (size_t i = 0; i < (rows - table_height) / 2; ++i) {
    printf("\n");
  }

  printf("%*c", (int)(cols - table_width) / 2, ' ');

  for (size_t i = 0; i < table->columns_count; ++i) {
    if (i > 0) {
      printf("|");
    }
    printf("%-*s", (int)cols_size[i], table->columns[i].name);
  }
  printf("\n");

  printf("%*c", (int)(cols - table_width) / 2, ' ');
  for (size_t i = 0; i < table->columns_count; ++i) {
    if (i > 0) {
      printf("*");
    }
    for (size_t j = 0; j < cols_size[i]; ++j) {
      printf("-");
    }
  }
  printf("\n");

  for (size_t i = 0; i < table->row_count; ++i) {
    printf("%*c", (int)(cols - table_width) / 2, ' ');
    for (size_t j = 0; j < table->columns_count; ++j) {
      if (j > 0) {
        printf("|");
      }
      print_value(table->rows[i].values[j], table->columns[j].type,
                  cols_size[j]);
    }
    printf("\n");
  }
  free(cols_size);
}
