
#ifndef PARSE_H
#define PARSE_H

#define MAX_COMMAND_LENGTH 1024

int parse_func(char*** argv,char* str, int idx);
void clean_input();
void sigint(int signum);
extern char input[MAX_COMMAND_LENGTH], *prompt_title; // Input buffer for current command
extern int input_length, retid;



#endif // PARSE_H