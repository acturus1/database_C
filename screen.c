#include "structs.c"
#include <stdio.h>
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
    printf("%d", value.int_val);
    break;
  case TYPE_FLOAT:
    printf("%f", value.float_val);
    break;
  case TYPE_DOUBLE:
    printf("%lf", value.double_val);
    break;
  case TYPE_STRING:
    printf("%s", value.string_val);
    break;
  case TYPE_BOOL:
    printf("%s", value.bool_val ? "true" : "false");
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

void print_text_on_center(const char *string, size_t string_len, size_t rows,
                          size_t cols) {
  clear_screen();
  for (size_t i = 0; i < rows / 2; ++i) {
    printf("\n");
  }
  if (string_len < cols) {
    size_t padding = (cols - string_len) / 2;
    for (size_t i = 0; i < padding; ++i) {
      printf(" ");
    }
  }
  printf("%s\n", string);
}

int main() {
  size_t a = 0, b = 0;
  //   get_screen_width(&a, &b);
  //   print_text_on_center("123", 3, a, b);
  //   while (1) {
  //   };
  DataValue value;
  value.double_val = 120.0;
  printf("%d", (int)get_value_size(value, 2));
  return 0;
}
