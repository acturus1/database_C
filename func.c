#include "structs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *create_table(const char *name, const char **col_names,
                    DataType *col_types, size_t col_count);
void print_value(DataValue value, DataType type);
void insert_row(Table *table, Row *row_insert);
DataValue get_cell_value(Table *table, size_t col_index, size_t row_index);
DataType get_cell_type(Table *table, size_t col_index);
void print_table(Table *table);
Database *create_database(const char *name);
void add_table_to_database(Database *database, Table *table);
void free_table(Table *table);
void free_database(Database *database);
void save_database_to_file(Database *database, char *filename);
void save_table_to_file(Table *table, FILE *file);
void save_row_to_file(Row row, Column *columns, size_t colimns_count,
                      FILE *file);
void save_value_to_file(DataValue value, DataType type, bool is_null,
                        FILE *file);
void read_database_from_file(Database *database, char *filename);

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
  table->name_length_table = strlen(name);

  table->row_count = 0;
  table->capacity = 10;
  table->rows = malloc(table->capacity * sizeof(Row));
  if (table->rows == NULL) {
    free_table(table);
    return NULL;
  }

  table->columns_count = col_count;
  table->columns = malloc(sizeof(Column) * col_count);
  if (table->columns == NULL) {
    free_table(table);
    return NULL;
  }
  for (size_t i = 0; i < col_count; ++i) {
    table->columns[i].name = strdup(col_names[i]);
    table->columns[i].name_length_column = strlen(col_names[i]);
    table->columns[i].type = col_types[i];
    if (table->columns[i].name == NULL) {
      free_table(table);
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
  }
}

void insert_row(Table *table, Row *row_insert) {
  if (table->capacity <= table->row_count) {
    table->capacity *= 2;
    table->rows = realloc(table->rows, table->capacity * sizeof(Row));
    if (table->rows == NULL) {
      return;
    }
  }
  // table->rows[table->row_count].values = values;
  Row *row = &table->rows[table->row_count];
  row->system_id = table->row_count + 1;
  row->values = malloc(table->columns_count * sizeof(DataValue));
  if (row->values == NULL) {
    return;
  }

  row->is_null = malloc(table->columns_count * sizeof(bool));
  if (row->is_null == NULL) {
    free(row->values);
    return;
  }
  for (size_t i = 0; i < table->columns_count; ++i) {
    row->is_null[i] = row_insert->is_null[i];
    if (!row->is_null[i]) {
      if (table->columns[i].type == TYPE_STRING) {
        row->values[i].string_val = strdup(row_insert->values[i].string_val);
      } else {
        row->values[i] = row_insert->values[i];
      }
    }
  }
  table->row_count++;
}

Database *create_database(const char *name) {
  Database *database = malloc(sizeof(Database));
  if (database == NULL) {
    return NULL;
  }
  database->name = strdup(name);
  if (database->name == NULL) {
    free_database(database);
    return NULL;
  }
  database->capacity = 10;
  database->table_count = 0;
  database->tables = malloc(database->capacity * sizeof(Table *));
  if (database->tables == NULL) {
    free_database(database);
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

void free_table(Table *table) {
  if (table == NULL) {
    return;
  }
  free(table->name);
  for (size_t i = 0; i < table->columns_count; ++i) {
    free(table->columns[i].name);
  }
  free(table->columns);
  for (size_t row = 0; row < table->row_count; ++row) {
    for (size_t j = 0; j < table->columns_count; ++j) {
      if (table->columns[j].type == TYPE_STRING) {
        free(table->rows[row].values[j].string_val);
      }
    }
    free(table->rows[row].values);
    free(table->rows[row].is_null);
  }
  free(table->rows);
  free(table);
}

void free_database(Database *database) {
  if (database == NULL) {
    return;
  }
  for (size_t i = 0; i < database->table_count; ++i) {
    free_table(database->tables[i]);
  }
  free(database->tables);
  free(database->name);
  free(database);
}

// void read_database_from_file(const char *filename) {
//   FILE *file = fopen(filename, "r");
//   if (!file) {
//     return;
//   }
//
//   if (fseek(file, 0, SEEK_END) != 0) {
//     return;
//   }
//   int file_size = ftell(file);
//   if (file_size == 0) {
//     return;
//   }
//   if (fseek(file, 0, SEEK_SET) != 0) {
//     return;
//   }
//
//   char *input = malloc(sizeof(char) * file_size);
//   while (fgets(input, file_size, file)) {
//     printf("%s", input);
//   }
//   fclose(file);
// }

void save_value_to_file(DataValue value, DataType type, bool is_null,
                        FILE *file) {
  if (is_null) {
    uint8_t is_null_int = 1;
    fwrite(&is_null_int, sizeof(uint8_t), 1, file);
  } else {
    uint8_t is_null_int = 0;
    fwrite(&is_null_int, sizeof(uint8_t), 1, file);
  }

  switch (type) {
  case TYPE_INT: {
    fwrite(&value.int_val, sizeof(uint32_t), 1, file);
    break;
  }
  case TYPE_FLOAT: {
    fwrite(&value.float_val, sizeof(float), 1, file);
    break;
  }
  case TYPE_DOUBLE: {
    fwrite(&value.double_val, sizeof(double), 1, file);
    break;
  }
  case TYPE_STRING: {
    uint64_t value_str_len = (uint64_t)strlen(value.string_val);
    fwrite(&value_str_len, sizeof(value_str_len), 1, file);
    fwrite(&value.string_val, value_str_len, 1, file);
    break;
  }
  case TYPE_BOOL: {
    uint8_t bool_to_int = (value.bool_val) ? 0 : 1;
    fwrite(&bool_to_int, sizeof(uint8_t), 1, file);
    break;
  }
  }
}

void save_row_to_file(Row row, Column *columns, size_t colimns_count,
                      FILE *file) {
  for (uint64_t i = 0; i < (uint64_t)colimns_count; ++i) {
    save_value_to_file(row.values[i], columns[i].type, row.is_null[i], file);
  }
}

void save_table_to_file(Table *table, FILE *file) {
  uint64_t table_name_length = (uint64_t)table->name_length_table;
  fwrite(&table_name_length, sizeof(uint64_t), 1, file);
  fwrite(table->name, table_name_length, 1, file);

  uint64_t table_col_count = (uint64_t)table->columns_count;
  fwrite(&table_col_count, sizeof(uint64_t), 1, file);

  for (uint64_t i = 0; i < table_col_count; ++i) {
    uint64_t column_name_length = table->columns[i].name_length_column;
    fwrite(&column_name_length, sizeof(uint64_t), 1, file);

    fwrite(table->columns[i].name, column_name_length, 1, file);

    uint8_t type_code =
        (uint8_t)table->columns[i]
            .type; // т.к у меня мало типов можно использовать uint8_t
    fwrite(&type_code, sizeof(type_code), 1, file);
  }

  uint64_t table_row_count = (uint64_t)table->row_count;
  fwrite(&table_row_count, sizeof(table_row_count), 1, file);

  for (uint64_t i = 0; i < table_row_count; ++i) {
    save_row_to_file(table->rows[i], table->columns, table->columns_count,
                     file);
  }
}

void save_database_to_file(Database *database, char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    return;
  }
  uint64_t code = 1234;
  fwrite(&code, sizeof(uint64_t), 1, file);

  uint32_t table_count = (uint32_t)database->table_count;
  fwrite(&table_count, sizeof(uint32_t), 1, file);

  uint64_t path_to_table_legend = 0;
  for (uint32_t i = 0; i < table_count; ++i) {
    fwrite(&path_to_table_legend, sizeof(uint64_t), 1, file);
  }

  uint64_t size_of_start_path_to_tables =
      (uint64_t)(sizeof(code) + sizeof(table_count));

  for (uint32_t i = 0; i < table_count; ++i) {
    uint64_t path_to_current_table = ftell(file);
    //   if (fseek(file,
    //             size_of_start_path_to_tables + (uint64_t)(sizeof(uint64_t)
    //             * i), SEEK_SET) != 0) {
    //     return;
    //   }
    fseek(file, size_of_start_path_to_tables + (uint64_t)(sizeof(uint64_t) * i),
          SEEK_SET);

    fwrite(&path_to_current_table, sizeof(uint64_t), 1,
           file); // эт кароче место в файле где находиться начало таблицы i

    fseek(file, path_to_current_table, SEEK_SET);
    save_table_to_file(database->tables[i], file);
  }

  fclose(file);
}

void read_database_from_file(Database *database, char *filename) {
  FILE *file = fopen(filename, "rb");
  database->name = filename;
  uint64_t tmp_uint64_t;
  fread(&tmp_uint64_t, sizeof(uint64_t), 1, file);
  uint64_t table_count;
  fread(&table_count, sizeof(uint64_t), 1, file);
  database->table_count = (size_t)table_count;

  fclose(file);
}
