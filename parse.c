/**
 * parse.c
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This file contains functions for parsing input commands in the shell,
 * cleaning input buffers, and handling signals like SIGINT.
 */

#include "parse.h"

/* parse_func
 * Parses a command string into separate arguments and stores them in a 2D array.
 * Params: char*** argv - Pointer to a 2D array to store the parsed arguments.
 *         char* string - The command string to parse.
 *         int idx - Index to store the parsed arguments in the argv array.
 * Returns: int - The number of arguments parsed.
 */
int parse_func(char*** argv,char* string, int idx){
    int i = 0;
    argv[idx] = (char **) malloc(10 * sizeof (char *));
    int length = strlen(string);
    while (length > 0 && string[length - 1] == ' ') {
        string[length - 1] = '\0';
        length--;
    }
    char *token;

    if (*string == ' ') string++;
    token = strtok(string, " ");
    while (token != NULL) {
        argv[idx][i++] = token;
        token = strtok(NULL, " ");
    }

    argv[idx][i] = NULL;
    return i;
}

/* clean_input
 * Clears the input buffer and resets the input length.
 */
void clean_input(){
    input[0] = '\0';
    input_length = 0;
    printf("%s:", prompt_title);
    fflush(stdout);

}

/* sigint
 * Signal handler for SIGINT (Ctrl+C). Cancels the current process if there is one.
 * Params: int signum - The signal number.
 */
void sigint(int signum) {
    printf("\r\033[K"); 
    if (retid > 0) {
        kill(retid, SIGINT);
        retid = 0;
    } else{
        printf("You typed Control-C!\n");
        printf("%s:", prompt_title);
        fflush(stdout);
    }
}

