#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Column Column;
typedef struct Table Table;
typedef struct Database Database;
typedef struct Row Row;

typedef enum { OK = 0, smth = 1 } Errors;

typedef enum {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_DOUBLE,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_NOON
} DataType;

typedef union {
  int int_val;
  float float_val;
  double double_val;
  char *string_val;
  bool bool_val;
} DataValue;

// Добавить ссылку на строку типа таблица на которую ссылаются (номер) и id
// строки, еще можно добавить в саму строку кто на нее ссылается
struct Row {
  DataValue *values;
};

struct Column {
  char *title;
  DataType type;
};

struct Table {
  char *name;
  size_t columns_count;
  Column *columns;

  size_t row_count;
  Row *rows;

  size_t capacity; // кол-во выделенной памяти
};

struct Database {
  Table *table[];
};

#endif
