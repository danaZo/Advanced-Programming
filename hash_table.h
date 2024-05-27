// hash_table.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define MAX_VAR_NAME_LEN 20
#define MAX_VAR_VALUE_LEN 50
#define HASH_TABLE_SIZE 100

typedef struct variable {
    char name[MAX_VAR_NAME_LEN];
    char value[MAX_VAR_VALUE_LEN];
    struct variable *next;
} Variable;

unsigned int hash_string(const char *str);
Variable *get_variable(const char *name);
void set_variable(const char *name, const char *value);
void free_hash_table();

#endif
