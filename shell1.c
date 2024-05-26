#include <sys/stat.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

int main() {
    int i, amper, retid, status;
    char *argv[10];
    char command[1024];
    char *token;

    while (1) {
        printf("hello: ");
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // Remove newline character

        /* Tokenize the input command */
        i = 0;
        token = strtok(command, " ");
        
        while (token != NULL) {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        
        argv[i] = NULL;

        /* Check if command is empty */ 
        if (argv[0] == NULL)
            continue;

        /* Check if the command ends with '&' */ 
        if (!strcmp(argv[i - 1], "&")) {
            amper = 1;
            argv[i - 1] = NULL;
        } else {
            amper = 0;
        } 

        /* Execute external commands */ 
        if (fork() == 0) { 
            execvp(argv[0], argv);
            // If execvp fails
            fprintf(stderr, "Error executing command: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Parent process waits for the child to complete if not running in background */
        if (amper == 0)
            wait(NULL);
    }
}
