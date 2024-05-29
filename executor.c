#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "hash_table.h"
#include "utils.h"
#include "parse.h"
#include "executor.h"

#define REDIRECT_OUT 1
#define REDIRECT_ERR 2
#define REDIRECT_APP 3
#define REDIRECT_IN 4

extern char *prompt_title;
extern int original_stdin, original_stdout, retid, status;
extern int input_length;
extern char input[MAX_COMMAND_LENGTH];
extern int changed_prompt, amper, redirect;
extern char **pipe_commands, ***args;
extern char *outfile;
extern int fd;


/**
 * Executes a command string `com` that may include multiple commands separated by pipes and various types of redirection.
 * This function handles command parsing, pipe creation, process forking, and I/O redirection based on the parsed commands.
 * It supports background execution, variable setting, changing working directories, and custom prompt settings among other features.
 * The function forks child processes for each command segment, setting up necessary pipes and redirects stdout, stderr, or stdin
 * as specified by redirection operators ('>', '>>', '<', '2>'). The `flag` parameter determines whether the last command's output
 * is redirected to `/dev/null`.
 * 
 * @param com The command string to execute, which can include pipes and redirection.
 * @param flag If non-zero, redirects the output of the last command in the pipeline to `/dev/null`.
 * @return The exit status of the last command executed or a failed status if an internal operation like fork or pipe fails.
 */


int exec_command(const char* com, int flag){
    int num_pipes = countCharOccurrences(com,'|');
    char* temp_input = strdup(com);
    int pipesfd[num_pipes][2];
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipesfd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    pipe_commands = (char**) malloc((num_pipes + 1) * sizeof (char *));
    char *token;
    token = strtok(temp_input,"|");
    int k = 0;
    while (token != NULL){
        pipe_commands[k++] = token;
        token = strtok(NULL, "|");
    }

    int pid,i;
    args = (char ***) malloc((num_pipes + 1) * sizeof(char **));
    int statusim[num_pipes+1];
    /*Action for each possible command*/
    for (int j = 0; j < num_pipes + 1; ++j) {
        i = parser(args,pipe_commands[j],j);
        if (args[j][0] == NULL)
            break;
        /* Does command line end with & */
        if (i>0 && !strcmp(args[j][i - 1], "&")) {
            amper = 1;
            args[j][i - 1] = NULL;
        }
        else
            amper = 0;

        if (i > 2 && args[j][0][0] == '$' && !strcmp(args[j][i - 2], "=")){   //Q10
            set_variable(args[j][i-3]+1,args[j][i-1]);
            statusim[j] = 0;
            continue;
        }

        if (! strcmp(args[j][0], "read")){
            input[0] = '\0';
            input_length = 0;
            char command[MAX_COMMAND_LENGTH];
            fgets(command, 1024, stdin);
            command[strlen(command)-1] = '\0';
            char new_word[20]; // allocate space for the new word
            strncpy(new_word, args[j][1], 20); // concatenate the original word to the new word
            set_variable(new_word,command);
            statusim[j] = 0;
            continue;
        }



        if (i>2 && ! strcmp(args[j][i - 3], "prompt") && (! strcmp(args[j][i - 2], "="))) {  //Q2
            if (changed_prompt) free(prompt_title);
            prompt_title = strdup(args[j][i - 1]);
            changed_prompt = 1;
            statusim[j] = 0;
            continue;
        } else if (i>1 && ! strcmp(args[j][0], "cd")) {   //Q5
            chdir(args[j][1]);
            statusim[j] = 0;
            continue;
        }
        if (i > 1 && !strcmp(args[j][i - 2], ">>")){    //Q1.2
            redirect = REDIRECT_APP;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }
        else if (i > 1 && !strcmp(args[j][i - 2], ">")) {
            redirect = REDIRECT_OUT;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }
        else if (i > 1 && !strcmp(args[j][i - 2], "<")) {
            redirect = REDIRECT_IN;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }
        else if (i > 1 && !strcmp(args[j][i - 2], "2>")) {    //Q1.1
            redirect = REDIRECT_ERR;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }
        else{
            redirect = 0;
        }
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            if (j > 0) {
                dup2(pipesfd[j-1][0], 0);
                close(pipesfd[j-1][0]);
                close(pipesfd[j-1][1]);
            }
            if (j < num_pipes) {
                // Redirect stdout to write end of pipe
                dup2(pipesfd[j][1], 1);
                close(pipesfd[j][0]);
                close(pipesfd[j][1]);
            }
            if (j == num_pipes && flag){
                int dev_null_fd = open("/dev/null", O_WRONLY);
                dup2(dev_null_fd, STDOUT_FILENO);
                close(dev_null_fd);
            }

            /* redirection of IO ? */
            if (redirect == REDIRECT_IN) {
                fd = open(outfile, O_RDONLY, 0660);
                close(STDIN_FILENO) ;
                dup(fd);
                close(fd);
                /* stdin is now redirected */
            }
            if (redirect == REDIRECT_OUT) {
                fd = creat(outfile, 0660);
                close(STDOUT_FILENO) ;
                dup(fd);
                close(fd);
                /* stdout is now redirected */
            }
            if (redirect == REDIRECT_ERR){
                if (freopen(outfile, "w", stderr) == NULL) {
                    perror("freopen error");
                    return 1;
                }
                // Restore stderr to its original stream
                if (freopen("/dev/stderr", "w", stderr) == NULL) {
                    perror("freopen error");
                    return 1;
                }
            }
            if (redirect == REDIRECT_APP){
                fd = open(outfile, O_CREAT | O_APPEND | O_RDWR, 0660);
                close(STDOUT_FILENO) ;
                dup(fd);
                close(fd);
            }
            if (! strcmp(args[j][0], "echo")) {   //Q3 && Q4
                if (args[j][1][0] == '$') {
                    if (args[j][1][1] == '?') {
                        printf("%d\n", status);
                    } else {
                        Variable *var = get_variable(args[j][1] + 1);
                        if (var)
                            printf("%s\n", var->value);
                    }
                } else {
                    for (int s = 1; s < i; s++) {
                        printf("%s ", args[j][s]);
                    }
                    printf("\n");
                }
                statusim[j] = 0;
                exit(1);
            }
            execvp(args[j][0], args[j]);
            exit(1);
        } else {
            // Parent process
            if (j < num_pipes) {
                // Close write end of pipe
                close(pipesfd[j][1]);
            }
            if (j > 0) {
                // Close read end of previous pipe
                close(pipesfd[j-1][0]);
                close(pipesfd[j-1][1]);
            }
            retid = pid;
            if (amper == 0){
                wait(&status);

            }
            statusim[j] = status;
        }
    }
    //redirect stdin and stdout to originals fds
    dup2(original_stdin, 0);
    dup2(original_stdout, 1);

    status = statusim[num_pipes];
    for (int i = 0; i < num_pipes+1; i++) {
        free(args[i]);
    }

    free(args);
    free(pipe_commands);
    free(temp_input);
    return status;
}
