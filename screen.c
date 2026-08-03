#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

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
  get_screen_width(&a, &b);
  print_text_on_center("123", 3, a, b);
  while (1) {
  };
  return 0;
}
