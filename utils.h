/**
 * utils.h
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This header file contains function prototypes for utility functions used in the shell,
 * including functions for interacting with the terminal, performing modular arithmetic,
 * counting occurrences of a character in a string, and displaying a command prompt.
*/
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
extern char *prompt_title;

/* get_char
 * Reads a single character from the terminal without echoing it.
 * Returns: char - The character read from the terminal.
*/
char get_char();

/* mod_func
 * Performs modular arithmetic on two integers.
 * Params: int a - The dividend.
 *         int b - The divisor.
 * Returns: int - The result of the modulo operation.
*/
int mod_func(int a, int b);

/* num_of_char
 * Counts the occurrences of a specified character in a string.
 * Params: const char* str - The string to search.
 *         char c - The character to count.
 * Returns: int - The number of occurrences of the specified character in the string.
*/
int num_of_char(const char* str, char c);

/* showCommand
 * Displays the command prompt and the current command being typed.
 * Params: char* command - The current command being typed.
*/
void showCommand(char* command);

#endif // UTILS_H
