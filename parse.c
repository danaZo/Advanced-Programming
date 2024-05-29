#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parse.h"



int parser(char*** argv,char* str, int idx){
    int i = 0;
    argv[idx] = (char **) malloc(10 * sizeof (char *));
    // Remove trailing spaces at the end of the input string
    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[len - 1] = '\0';
        len--;
    }
    char *token;
    if (*str == ' ') str++;
    token = strtok(str, " ");
    while (token != NULL) {
        argv[idx][i++] = token;
        token = strtok(NULL, " ");
    }
    argv[idx][i] = NULL;
    return i;
}

void cleanInput(){
    input[0] = '\0';
    input_length = 0;

    printf("%s:", prompt_name);
    fflush(stdout);
}

void sigint_handler(int signum) {
    printf("\r\033[K"); // Clear the current line
    if (retid > 0) {
        // Send SIGINT to the child process
        kill(retid, SIGINT);
        retid = 0;
    } else{
        printf("You typed Control-C!\n");
        printf("%s:", prompt_name);
        fflush(stdout);
    }
}

