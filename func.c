#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *create_table(const char *name, const char **col_names,
                    DataType *col_types, size_t col_count) {
  Table *table = malloc(sizeof(Table));
  if (table == NULL) {
    return NULL;
  };

  table->name = strdup(name);
  table->row_count = 0;
  table->capacity = 10;
  table->rows = malloc(table->capacity * sizeof(Row));
  if (table->rows == NULL) {
    free(table->name);
    free(table);
    return NULL;
  }

  table->columns_count = col_count;
  table->columns = malloc(sizeof(Column) * col_count);
  if (table->columns == NULL) {
    free(table->rows);
    free(table->name);
    free(table);
    return NULL;
  }
  for (size_t i = 0; i < col_count; ++i) {
    table->columns[i].title = strdup(col_names[i]);
    table->columns[i].type = col_types[i];
    if (table->columns[i].title == NULL) {
      for (size_t j = 0; j < i; ++j) {
        free(table->columns[j].title);
      }
      free(table->columns);
      free(table->name);
      free(table);
      return NULL;
    }
  }
  return table;
};
