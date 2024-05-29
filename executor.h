/**
 * executor.h
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This header file defines the structures and function prototypes for executing commands
 * in the shell. It includes definitions for function prototypes for command execution,
 * file redirection, and other related functionalities.
*/

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "hash_table.h"
#include "utils.h"
#include "parse.h"

#define OUT_FILE_REDIRECT 1
#define ERROR_FILE_REDIRECT 2
#define APPEND_FILE_REDIRECT 3
#define INPUT_FILE_REDIRECT 4


/* exec_command
 * Executes a command string that may include pipes and various types of redirection.
 * Params: const char* com - The command string to execute.
 *         int flag - If non-zero, redirects the output of the last command in the pipeline to `/dev/null`.
 * Returns: int - The exit status of the last command executed or a failed status if an internal operation fails.
*/
int exec_command(const char* com, int flag);

#endif // EXECUTOR_H
