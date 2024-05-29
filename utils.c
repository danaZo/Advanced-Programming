
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "utils.h"


// Function to read a single character from the terminal without waiting for the Enter key to be pressed
char get_char(){
    // Initialize values
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

// Mod function
int mod_func(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}

// This function counts number of character occurences in given string
int num_of_char(const char* string, char c) {
    int count = 0;
    // Run through string
    while (*string != '\0') {
        // If sees char then add to count
        if (*string == c) {
            count++;
        }
        string++;
    }
    return count;
}

void showCommand(char* command){
    // Clear the current line
    printf("\r\033[K"); 
    // Print the prompt and command
    printf("%s:%s", prompt_title, command);
    fflush(stdout);
}
