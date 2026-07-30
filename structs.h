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
  TYPE_BOOL
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
  bool *is_null;
};

struct Column {
  size_t name_length_column; // без \0
  char *name;

  DataType type;
};

struct Table {
  size_t name_length_table; // без \0
  char *name;

  size_t columns_count;
  Column *columns;

  size_t row_count;
  Row *rows;

  size_t capacity; // кол-во выделенной памяти
};

struct Database {
  char *name;

  size_t capacity;
  size_t table_count;
  Table **tables; // массив указателей на tables
};

#endif
