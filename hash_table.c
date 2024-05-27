// hash_table.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

Variable *hash_table[HASH_TABLE_SIZE];

// Hash function for strings
unsigned int hash_string(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }
    return hash % HASH_TABLE_SIZE;
}

// Get a variable from the hash table
Variable *get_variable(const char *name) {
    unsigned int hash = hash_string(name);
    Variable *var = hash_table[hash];
    while (var != NULL) {
        if (strcmp(var->name, name) == 0) {
            return var;
        }
        var = var->next;
    }
    return NULL;
}

// Set the value of a variable
void set_variable(const char *name, const char *value) {
    Variable *var = get_variable(name);
    if (var == NULL) {
        // Variable does not exist, create a new one
        unsigned int hash = hash_string(name);
        var = (Variable *)malloc(sizeof(Variable));
        strncpy(var->name, name, MAX_VAR_NAME_LEN);
        var->next = hash_table[hash];
        hash_table[hash] = var;
    }
    strncpy(var->value, value, MAX_VAR_VALUE_LEN);
}

// Free the hash table
void free_hash_table() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Variable *currNode = hash_table[i];
        while (currNode != NULL) {
            Variable *nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
    }
}
