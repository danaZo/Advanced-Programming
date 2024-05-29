#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parse.h"



// In this function we parse from the user's input and clean the string

int parser(char*** argv,char* string, int idx){
    // Initialize values
    int i = 0;
    argv[idx] = (char **) malloc(10 * sizeof (char *));

    // Remove unnecessary spaces at the end of the input
    int length = strlen(string);
    // Go through string
    while (length > 0 && string[length - 1] == ' ') {
        string[length - 1] = '\0';
        length--;
    }
    // Define token
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

// In this functionwe clean the input that was entered by the user

void cleanInput(){

// Initialize valuess
    input[0] = '\0';
    input_length = 0;
// Print prompt
    printf("%s:", prompt_title);
    fflush(stdout);

}

// Response to esc, CTRL C 

void sigint_handler(int signum) {
    // Clear the current line
    printf("\r\033[K"); 
    if (retid > 0) {
        // Send SIGINT to child process
        kill(retid, SIGINT);
        retid = 0;
    } else{
        // If we get CTRL C then print this

        printf("You typed Control-C!\n");
        printf("%s:", prompt_title);
        fflush(stdout);
    }
}

