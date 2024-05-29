/**
 * myshell.h
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This header file declares function prototypes, global variables, and macros
 * for the main functionality of the shell program
*/

#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include "hash_table.h"
#include "utils.h"
#include "parse.h"
#include "executor.h"

#define MAX_COMMANDS 20

extern char *outfile, *last_command, *prompt_title;
extern char ***args, **pipe_commands;
extern char *commands[MAX_COMMANDS];
extern char input[MAX_COMMAND_LENGTH];
extern int input_length, command_index, num_commands;
extern int fd, amper, redirect, retid, status, changed_prompt, changed_last, original_stdin, original_stdout;

#endif // MYSHELL_H
