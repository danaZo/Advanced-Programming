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
Variable *variables[HASH_TABLE_SIZE];

/* compute_hash
 * Computes the hash value for a given string.
 * Implementation: Uses a simple hash function that multiplies and adds character values.
 * Params: const char *str - The string to hash.
 * Returns: unsigned int - The computed hash value.
 */
unsigned int compute_hash(const char *string) {
    unsigned int h = 0;
    while (*string) {
        h = h * 31 + *string;
        string++;
    }
    return h % HASH_TABLE_SIZE;
}

/* get_var
 * Retrieves a variable from the hash table by its name.
 * Implementation: Computes the hash and traverses the linked list at that hash index.
 * Params: const char *name - The name of the variable to retrieve.
 * Returns: Variable* - A pointer to the variable struct, or NULL if not found.
 */
Variable *get_var(const char *title) {
    unsigned int hash = compute_hash(title);
    Variable *var = variables[hash];
    while (var != NULL) {
        if (strcmp(var->name, title) == 0) {
            return var;
        }
        var = var->next;
    }
    return NULL;
}

/* set_var
 * Sets the value of a variable in the hash table. If the variable does not exist, it creates a new one.
 * Implementation: Computes the hash, creates a new variable if necessary, and sets the value.
 * Params: const char *name - The name of the variable to set.
 *         const char *value - The value to set for the variable.
 */
void set_var(const char *title, const char *val) {
    Variable *var = get_var(title);
    if (var == NULL) {
        unsigned int hash = compute_hash(title);
        var = (Variable *)malloc(sizeof(Variable));
        strncpy(var->name, title, MAX_VAR_NAME_LEN);
        var->next = variables[hash];
        variables[hash] = var;
    }
    strncpy(var->value, val, MAX_VAR_VALUE_LEN);
}

/* Free_variables
 * Frees all memory allocated for the hash table.
 * Implementation: Iterates over each hash index and frees all variables in the linked list.
 */
void free_variables() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Variable *currNode = variables[i];
        while (currNode != NULL) {
            Variable *nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
    }
}