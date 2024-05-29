/**
 * utils.c
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This file contains implementations of utility functions for the shell,
 * including functions for getting a single character from the terminal,
 * performing modular arithmetic, counting occurrences of a character in a string,
 * and displaying a command prompt.
*/

#include "utils.h"

/* get_char
 * Reads a single character from the terminal without echoing it.
 * Returns: char - The character read from the terminal.
*/
char get_char(){
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

/* mod_func
 * Performs modular arithmetic on two integers.
 * Params: int a - The dividend.
 *         int b - The divisor.
 * Returns: int - The result of the modulo operation.
*/
int mod_func(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}

/* num_of_char
 * Counts the occurrences of a specified character in a string.
 * Params: const char* string - The string to search.
 *         char c - The character to count.
 * Returns: int - The number of occurrences of the specified character in the string.
*/
int num_of_char(const char* string, char c) {
    int count = 0;
    while (*string != '\0') {
        if (*string == c) {
            count++;
        }
        string++;
    }
    return count;
}

/* showCommand
 * Displays the command prompt and the current command being typed.
 * Params: char* command - The current command being typed.
*/
void showCommand(char* command){
    printf("\r\033[K");
    printf("%s:%s", prompt_title, command);
    fflush(stdout);
}
