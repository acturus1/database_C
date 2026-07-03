#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *create_table(const char *name, const char **col_names,
                    DataType *col_types, size_t col_count);
void print_value(DataValue value, DataType type);
void insert_row(Table *table, DataValue *values);
DataValue get_cell_value(Table *table, size_t col_index, size_t row_index);
DataType get_cell_type(Table *table, size_t col_index);
void print_table(Table *table);
Database *create_database(const char *name);
void add_table_to_database(Database *database, Table *table);

Table *create_table(const char *name, const char **col_names,
                    DataType *col_types, size_t col_count) {
  Table *table = malloc(sizeof(Table));
  if (table == NULL) {
    return NULL;
  };

  table->name = strdup(name);
  if (table->name == NULL) {
    free(table);
    return NULL;
  }
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
}

void print_value(DataValue value, DataType type) {
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
  case TYPE_NULL:
    printf("NULL");
    break;
  }
}

void insert_row(Table *table, DataValue *values) {
  if (table->capacity <= table->row_count) {
    table->capacity *= 2;
    table->rows = realloc(table->rows, table->capacity * sizeof(Row));
    if (table->rows == NULL) {
      return;
    }
  }
  // table->rows[table->row_count].values = values;
  Row *row = &table->rows[table->row_count];
  row->values = malloc(table->columns_count * sizeof(DataValue));
  if (row->values == NULL) {
    return;
  }
  for (size_t i = 0; i < table->columns_count; ++i) {
    if (table->columns[i].type == TYPE_STRING) {
      row->values[i].string_val = strdup(values[i].string_val);
    } else {
      row->values[i] = values[i];
    }
  }
  table->row_count++;
}

DataValue get_cell_value(Table *table, size_t col_index, size_t row_index) {
  if (row_index >= table->row_count || col_index >= table->columns_count) {
    DataValue none = {0};
    return none;
  }
  return table->rows[row_index].values[col_index];
}

DataType get_cell_type(Table *table, size_t col_index) {
  if (col_index >= table->columns_count) {
    return TYPE_NULL;
  }
  return table->columns[col_index].type;
}

void print_table(Table *table) {
  for (size_t row = 0; row < table->row_count; row++) {
    for (size_t col = 0; col < table->columns_count; col++) {
      print_value(get_cell_value(table, col, row), get_cell_type(table, col));
    }
  }
}

Database *create_database(const char *name) {
  Database *database = malloc(sizeof(Database));
  if (database == NULL) {
    return NULL;
  }
  database->name = strdup(name);
  if (database->name == NULL) {
    free(database);
    return NULL;
  }
  database->capacity = 10;
  database->table_count = 0;
  database->tables = malloc(database->capacity * sizeof(Table *));
  if (database->tables == NULL) {
    free(database->name); // т.к strdup выделил память под name
    free(database);
    return NULL;
  }
  return database;
}

void add_table_to_database(Database *database, Table *table) {
  if (database == NULL || table == NULL) {
    return;
  }
  if (database->table_count >= database->capacity) {
    database->capacity *= 2;
    Table **new_tables =
        realloc(database->tables, sizeof(Table *) * database->capacity);
    if (new_tables == NULL) {
      return;
    }
    database->tables = new_tables;
  }
  database->tables[database->table_count] = table;
  database->table_count++;
}
