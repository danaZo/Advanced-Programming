/**
 * executor.c
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This file implements the command execution functionality for the shell.
 * It handles the parsing of commands, setting up pipes and redirections,
 * forking child processes, and executing commands. 
 * The file supports background execution, setting shell variables, changing directories,
 * and custom prompt settings.
*/

#include "executor.h"

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
    /**
     * Initialize pipes for command execution.
     * 
     * First, count the number of pipe ('|') characters in the command string `com` 
     * to determine how many pipes are needed. Then, duplicate the command string 
     * `com` into `temp_input` to avoid modifying the original string during processing.
     * 
     * Next, declare an array `pipesfd` to hold the file descriptors for each pipe.
     * Create the required number of pipes using a loop. Each pipe consists of a 
     * pair of file descriptors: one for reading and one for writing. If the pipe 
     * creation fails, print an error message and exit the program.
    */
    int num_pipes = num_of_char(com,'|');
    char* temp_input = strdup(com);
    int pipesfd[num_pipes][2];
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipesfd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    /**
     * Split the input command string into individual commands.
     * 
     * Allocate memory for an array of strings, `pipe_commands`, which will hold 
     * each individual command separated by pipes. The number of commands is 
     * `num_pipes + 1` because there is one more command than there are pipes.
     * 
     * Use `strtok` to tokenize the duplicated command string `temp_input` based on the pipe ('|') character.
     * The `strtok` function splits the string into tokens, each representing a command.
     * 
     * Iterate through the tokens, storing each one in the `pipe_commands` array.
     * The loop continues until `strtok` returns `NULL`, indicating that there are no more tokens.
    */
    pipe_commands = (char**) malloc((num_pipes + 1) * sizeof (char *));
    char *token;
    token = strtok(temp_input,"|");
    int k = 0;
    while (token != NULL){
        pipe_commands[k++] = token;
        token = strtok(NULL, "|");
    }

    /**
     * Declare variables `pid` for process ID and `i` for indexing.
     * Allocate memory for `args`, a 3D array of strings, which will hold the parsed 
     * arguments for each command. The size of `args` is `num_pipes + 1` because 
     * there are `num_pipes + 1` commands.
     * 
     * Declare an array `command_status` to store the status of each command execution.
     * The size of `command_status` is also `num_pipes + 1`.
     * 
     * Iterate over each command (from `pipe_commands`) to parse its arguments.
     * Use the `parse_func` function to fill `args` with the arguments of each command.
     * 
     * @param int pid - Process ID for child processes.
     * @param int i - Index variable.
     * @param char ***args - 3D array to store arguments for each command.
     * @param int command_status[] - Array to store the status of each command execution.
     * @param int j - Loop index for iterating over commands.
    */
    int pid,i;
    args = (char ***) malloc((num_pipes + 1) * sizeof(char **));
    int command_status[num_pipes+1];
    for (int j = 0; j < num_pipes + 1; ++j) {

        /**
         * Parse the command string and store the parsed arguments in args[j].
         * If there are no arguments parsed for the current command, 
         * break out of the loop.
        */
        i = parse_func(args,pipe_commands[j],j);
        if (args[j][0] == NULL)
            break;

        /**
         * If the last argument of the current command is "&",
         * set the amper flag to 1 to indicate background execution,
         * and replace the "&" with NULL to terminate the argument list properly.
        */    
        if (i>0 && !strcmp(args[j][i - 1], "&")) {
            amper = 1;
            args[j][i - 1] = NULL;
        }

        /**
         * If the last argument of the current command is not "&",
         * set the amper flag to 0 to indicate foreground execution.
        */
        else
            amper = 0;

        /**
         * If the command starts with '$', indicating a variable assignment,
         * and the second to last argument is "=", set the variable indicated
         * by the first argument to the value indicated by the last argument.
        */
        if (i > 2 && args[j][0][0] == '$' && !strcmp(args[j][i - 2], "=")){   
            set_var(args[j][i-3]+1,args[j][i-1]);
            command_status[j] = 0;
            continue;
        }

        /**
         * If the command is "read", indicating a variable assignment from user input:
         * - Clear the input buffer and reset its length.
         * - Prompt the user for input using fgets, truncating at 1024 characters.
         * - Set the variable indicated by the second argument to the user input.
        */
        if (! strcmp(args[j][0], "read")){
            input[0] = '\0';
            input_length = 0;
            char command[MAX_COMMAND_LENGTH];
            fgets(command, 1024, stdin);
            command[strlen(command)-1] = '\0';
            char new_word[20];
            strncpy(new_word, args[j][1], 20); 
            set_var(new_word,command);
            command_status[j] = 0;
            continue;
        }

        /**
         * If the command is setting a new prompt title:
         * - Free memory if a previous prompt title was set.
         * - Set the prompt title to the new value.
         * - Update the flag indicating a change in the prompt.
        */
        if (i>2 && ! strcmp(args[j][i - 3], "prompt") && (! strcmp(args[j][i - 2], "="))) {  
            if (changed_prompt) free(prompt_title);
            prompt_title = strdup(args[j][i - 1]);
            changed_prompt = 1;
            command_status[j] = 0;
            continue;   
        } 
        
        /**
         * If the command is changing the directory:
         * - Change the current working directory to the specified directory.
         * - Update the command status to indicate successful execution.
        */
        else if (i>1 && ! strcmp(args[j][0], "cd")) {   
            chdir(args[j][1]);
            command_status[j] = 0;
            continue;
        }
        
        /**
         * If the command contains the ">>" redirection operator:
         * - Set the redirection type to append output to a file.
         * - Set the output file name to the argument before the ">>" operator.
        */
        if (i > 1 && !strcmp(args[j][i - 2], ">>")){    
            redirect = APPEND_FILE_REDIRECT;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }

        /**
         * If the command contains a '>' operator, 
         * it indicates output redirection to a file.
         * Set the redirect type to redirect output to a file.
         * Clear the redirection operator from the command arguments and set the output file name.
        */
        else if (i > 1 && !strcmp(args[j][i - 2], ">")) {
            redirect = OUT_FILE_REDIRECT;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }

        /**
         * If the command contains a '<' operator, 
         * it indicates input redirection from a file.
         * Set the redirect type to redirect input from a file.
         * Clear the redirection operator from the command arguments and set the input file name.
        */
        else if (i > 1 && !strcmp(args[j][i - 2], "<")) {
            redirect = INPUT_FILE_REDIRECT;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }
        
        /**
         * If the command contains a '2>' operator, 
         * it indicates redirection of stderr to a file.
         * Set the redirect type to redirect stderr to a file.
         * Clear the redirection operator from the command arguments and set the output file name.
        */
        else if (i > 1 && !strcmp(args[j][i - 2], "2>")) {   
            redirect = ERROR_FILE_REDIRECT;
            args[j][i - 2] = NULL;
            outfile = args[j][i - 1];
        }

        /**
         * If no redirection operator is found in the command,
         * set the redirect type to indicate no redirection (0).
        */
        else{
            redirect = 0;
        }

        /**
         * Forks a child process to execute the current command segment.
         * If the fork fails, an error message is printed, 
         * and the program exits with status 1.
        */
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }

        /**
         * If the process ID is 0, it means this code is executing in the child process.
         * This block of code handles the actions that the child process should take.
        */
        else if (pid == 0) {
            
            /**
             * If the current command is not the FIRST command in the pipeline:
             * - Redirects the standard input of the child process to the read end of the previous pipe.
             * - Closes the read end of the previous pipe as it's no longer needed by this process.
             * - Closes the write end of the previous pipe as it's also not needed by this process.
            */
            if (j > 0) {
                dup2(pipesfd[j-1][0], 0);
                close(pipesfd[j-1][0]);
                close(pipesfd[j-1][1]);
            }

            /**
             * If the current command is not the LAST command in the pipeline:
             * - Redirects the standard output of the child process to the write end of the current pipe.
             * - Closes the read end of the current pipe as it's no longer needed by this process.
             * - Closes the write end of the current pipe as it's already been duplicated for the child process.
            */
            if (j < num_pipes) {
                dup2(pipesfd[j][1], 1);
                close(pipesfd[j][0]);
                close(pipesfd[j][1]);
            }

            /**
             * Redirects the output of the last command in the pipeline to /dev/null if the flag is set.
             * This is done to discard the output if it's not needed.
            */
            if (j == num_pipes && flag){
                int dev_null_fd = open("/dev/null", O_WRONLY);
                dup2(dev_null_fd, STDOUT_FILENO);
                close(dev_null_fd);
            }

            /**
             * If the command requires input redirection from a file:
             * - Opens the specified file for reading only.
             * - Closes the standard input file descriptor (0).
             * - Duplicates the file descriptor of the opened file to the standard input file descriptor.
             * - Closes the duplicate file descriptor.
            */
            if (redirect == INPUT_FILE_REDIRECT) {
                fd = open(outfile, O_RDONLY, 0660);
                close(STDIN_FILENO) ;
                dup(fd);
                close(fd);
            }

            /**
             * If the command requires output redirection to a file:
             * - Creates the specified file with write permissions.
             * - Closes the standard output file descriptor (1).
             * - Duplicates the file descriptor of the created file to the standard output file descriptor.
             * - Closes the duplicate file descriptor.
            */
            if (redirect == OUT_FILE_REDIRECT) {
                fd = creat(outfile, 0660);
                close(STDOUT_FILENO) ;
                dup(fd);
                close(fd);
            }

            /**
             * If the command requires redirection of stderr to a file:
             * - Attempts to reopen the specified file for writing and redirects stderr to it.
             * - If the freopen operation fails, prints an error message and returns with a failure status.
             * - If reopening '/dev/stderr' for writing also fails, prints an error message and returns with a failure status.
            */
            if (redirect == ERROR_FILE_REDIRECT){
                if (freopen(outfile, "w", stderr) == NULL) {
                    perror("freopen error");
                    return 1;
                }
                if (freopen("/dev/stderr", "w", stderr) == NULL) {
                    perror("freopen error");
                    return 1;
                }
            }

            /**
             * If the command requires appending output to a file:
             * - Opens the specified file for reading and writing, creating it if it doesn't exist and appending data to it.
             * - Closes the standard output file descriptor, 
             * duplicates the file descriptor of the opened file, 
             * and then closes the duplicated descriptor.
             * This redirects the standard output to the opened file for appending.
            */
            if (redirect == APPEND_FILE_REDIRECT){
                fd = open(outfile, O_CREAT | O_APPEND | O_RDWR, 0660);
                close(STDOUT_FILENO) ;
                dup(fd);
                close(fd);
            }
            
            /**
             * If the command is "echo":
             * - If the argument starts with '$':
             *   - If the next character is '?', prints the exit status of the last command.
             *   - Otherwise, attempts to retrieve the value of the variable associated with the argument and prints it.
             * - If the argument doesn't start with '$', prints all arguments followed by a newline.
             * Sets the command status for the current command to 0 and exits.
             * If the command is not "echo", attempts to execute it with the given arguments using execvp.
             * Exits the child process upon completion.
            */
            if (! strcmp(args[j][0], "echo")) {   
                if (args[j][1][0] == '$') {
                    if (args[j][1][1] == '?') {
                        printf("%d\n", status);
                    } else {
                        Variable *var = get_var(args[j][1] + 1);
                        if (var)
                            printf("%s\n", var->value);
                    }
                } else {
                    for (int s = 1; s < i; s++) {
                        printf("%s ", args[j][s]);
                    }
                    printf("\n");
                }
                command_status[j] = 0;
                exit(1);
            }
            execvp(args[j][0], args[j]);
            exit(1);
        } 
        
        /**
         * Parent process block:
         * - If the current command is not the last command in the pipeline:
         *   - Close the write end of the pipe associated with the current command.
         * - If the current command is not the first command in the pipeline:
         *   - Close the read end of the pipe associated with the previous command.
         * - Set the retid to the process ID of the current command.
         * - If the command is not intended for background execution:
         *   - Wait for the child process to finish and update the status.
         * Set the command status for the current command to the status of the child process.
        */
        else {
            if (j < num_pipes) {
                close(pipesfd[j][1]);
            }
            if (j > 0) {
                close(pipesfd[j-1][0]);
                close(pipesfd[j-1][1]);
            }
            retid = pid;
            if (amper == 0){
                wait(&status);

            }
            command_status[j] = status;
        }
    }
    
    /**
     * Restore the original standard input and output file descriptors.
     * Set the status to the status of the last command executed.
     * Free memory allocated for command arguments and pipe commands.
     * Return the status of the last command executed.
    */
    dup2(original_stdin, 0);
    dup2(original_stdout, 1);
    status = command_status[num_pipes];
    for (int i = 0; i < num_pipes+1; i++) {
        free(args[i]);
    }
    free(args);
    free(pipe_commands);
    free(temp_input);
    return status;
}
