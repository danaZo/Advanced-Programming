/**
 * parse.h
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This header file defines function prototypes and macros for parsing input commands,
 * cleaning input buffers, and handling signals in the shell.
*/
#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
extern char input[MAX_COMMAND_LENGTH], *prompt_title; 
extern int input_length, retid;

/* parse_func
 * Parses a command string into separate arguments and stores them in a 2D array.
 * Params: char*** argv - Pointer to a 2D array to store the parsed arguments.
 *         char* string - The command string to parse.
 *         int idx - Index to store the parsed arguments in the argv array.
 * Returns: int - The number of arguments parsed.
*/
int parse_func(char*** argv,char* str, int idx);

/* clean_input
 * Clears the input buffer and resets the input length.
*/
void clean_input();

/* sigint
 * Signal handler for SIGINT (Ctrl+C). Cancels the current process if there is one.
 * Params: int signum - The signal number.
*/
void sigint(int signum);

#endif // PARSE_H