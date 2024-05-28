/**
 * hash_table.c
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This file implements a hash table for storing and retrieving shell variables.
 * It includes functions for hashing strings, setting and getting variables, and freeing the hash table.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

/* hash_table
 * A global hash table for storing variables.
 */
Variable *hash_table[HASH_TABLE_SIZE];

/* hash_string
 * Computes the hash value for a given string.
 * Implementation: Uses a simple hash function that multiplies and adds character values.
 * Params: const char *str - The string to hash.
 * Returns: unsigned int - The computed hash value.
 */
unsigned int hash_string(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }
    return hash % HASH_TABLE_SIZE;
}

/* get_variable
 * Retrieves a variable from the hash table by its name.
 * Implementation: Computes the hash and traverses the linked list at that hash index.
 * Params: const char *name - The name of the variable to retrieve.
 * Returns: Variable* - A pointer to the variable struct, or NULL if not found.
 */
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

/* set_variable
 * Sets the value of a variable in the hash table. If the variable does not exist, it creates a new one.
 * Implementation: Computes the hash, creates a new variable if necessary, and sets the value.
 * Params: const char *name - The name of the variable to set.
 *         const char *value - The value to set for the variable.
 */
void set_variable(const char *name, const char *value) {
    Variable *var = get_variable(name);
    if (var == NULL) {
        unsigned int hash = hash_string(name);
        var = (Variable *)malloc(sizeof(Variable));
        strncpy(var->name, name, MAX_VAR_NAME_LEN);
        var->next = hash_table[hash];
        hash_table[hash] = var;
    }
    strncpy(var->value, value, MAX_VAR_VALUE_LEN);
}

/* free_hash_table
 * Frees all memory allocated for the hash table.
 * Implementation: Iterates over each hash index and frees all variables in the linked list.
 */
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