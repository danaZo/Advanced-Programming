#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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

