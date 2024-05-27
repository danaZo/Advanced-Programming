
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "utils.h"


// Function to read a single character from terminal without waiting for Enter key
char getch(){
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

int mod(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}

int countCharOccurrences(const char* str, char c) {
    int count = 0;
    while (*str != '\0') {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}

void display_command(char* command){
    printf("\r\033[K"); // Clear the current line
    printf("%s:%s", prompt_name, command);
    fflush(stdout);
}
