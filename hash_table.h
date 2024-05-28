/**
 * hash_table.h
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This header file defines the structures and function prototypes for a hash table
 * that stores and retrieves shell variables. It includes definitions for the variable
 * structure and function prototypes for string hashing, setting and getting variables, 
 * and freeing the hash table.
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define MAX_VAR_NAME_LEN 20
#define MAX_VAR_VALUE_LEN 50
#define HASH_TABLE_SIZE 100

/* Variable
 * Structure to represent a variable in the hash table.
 * Fields:
 *   - name: The name of the variable.
 *   - value: The value of the variable.
 *   - next: Pointer to the next variable in the linked list (for collision handling).
 */
typedef struct variable {
    char name[MAX_VAR_NAME_LEN];
    char value[MAX_VAR_VALUE_LEN];
    struct variable *next;
} Variable;

/* compute_hash
 * Computes the hash value for a given string.
 * Params: const char *str - The string to hash.
 * Returns: unsigned int - The computed hash value.
 */
unsigned int compute_hash(const char *str);

/* get_variable
 * Retrieves a variable from the hash table by its name.
 * Params: const char *name - The name of the variable to retrieve.
 * Returns: Variable* - A pointer to the variable struct, or NULL if not found.
 */
Variable *get_variable(const char *name);

/* set_variable
 * Sets the value of a variable in the hash table. If the variable does not exist, it creates a new one.
 * Params: const char *name - The name of the variable to set.
 *         const char *value - The value to set for the variable.
 */
void set_variable(const char *name, const char *value);

/* free_hash_table
 * Frees all memory allocated for the hash table.
 */
void free_hash_table();

#endif