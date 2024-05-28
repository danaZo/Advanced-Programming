#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "errno.h"
#include <signal.h>
#include <ctype.h> 
#include "hash_table.h"
#include "utils.h"
#include "parse.h"
#include "executor.h"

#define MAX_COMMANDS 20

extern char *outfile, *last_command, *prompt_name;
extern char ***args, **pipe_commands, *commands[MAX_COMMANDS];
extern char input[MAX_COMMAND_LENGTH];
extern int input_length, command_index, num_commands;
extern int fd, amper, redirect, retid, status, changed_prompt, changed_last, orig_stdin, orig_stdout;

void cleanInput();
void sigint_handler(int signum);

#endif // MYSHELL_H
