/**
 * myshell.c
 * 
 * Implementation By: Dana Zorohov, Noa Nussbaum
 * 
 * This file implements the main functionality of the shell program.
 * It handles user input, command execution, command history, and other shell features.
 * The shell supports built-in commands, external commands, command history navigation,
 * and customizable prompt settings.
 */

#include "myshell.h"

/**
 * Global Variables:
 * 
 * - outfile: Stores the name of the output file for redirection.
 * - last_command: Stores the last command executed in the shell.
 * - prompt_title: Stores the title of the shell prompt.
 * - args: Pointer to a pointer to a pointer to store command arguments.
 * - pipe_commands: Pointer to a pointer to store commands in a pipeline.
 * - commands: Array to store command history.
 * - input: Buffer to store user input for the current command.
 * - input_length: Length of the current input buffer.
 * - command_index: Index of the current command in the command history.
 * - num_commands: Total number of commands in the command history.
 * - fd: File descriptor for file operations.
 * - amper: Flag to indicate background execution of commands.
 * - redirect: Type of redirection (if any) for command execution.
 * - retid: Process ID of the last executed command.
 * - status: Status of the last executed command.
 * - changed_prompt: Flag to indicate if the prompt title has been changed.
 * - changed_last: Flag to indicate if the last command has been changed.
 * - original_stdin: File descriptor for the original standard input.
 * - original_stdout: File descriptor for the original standard output.
*/
char *outfile,*last_command = "";

char *prompt_title = "hello";

char ***args,**pipe_commands;

char *commands[MAX_COMMANDS]; 

char input[MAX_COMMAND_LENGTH] = ""; 

int input_length = 0, command_index = 0;

int num_commands = 0;

int fd, amper,redirect,retid,status,changed_prompt,changed_last,original_stdin,original_stdout;

/**
 * Main Function:
 * 
 * - Sets up signal handling for SIGINT (Ctrl+C).
 * - Prints the shell prompt title.
 * - Saves the original standard input and output file descriptors.
 */
int main(){
    signal(SIGINT, sigint);
    printf("%s:",prompt_title);
    fflush(stdout);
    original_stdin = dup(0);
    original_stdout = dup(1);

    /**
     * - Enters an infinite loop to continuously read input characters.
     * - Uses get_char() to read a character from the terminal.
    */
    while (1){
        char c = get_char(); 

        /**
        * If the character is a newline ('\n'), prints a newline to move to the next line.
        */
        if (c == '\n'){
            printf("\n");

            /**
             * Input Check:
             * 
             * - Checks if the input length is zero.
             * - If so, calls clean_input() to clear the input buffer and prompt.
            */
            if (input_length == 0){
                clean_input();
                continue;
            }

            /**
             * Quit Command Check:
             * 
             * - Checks if the input string is equal to "quit".
             * - If true, exits the loop to terminate the program.
            */
            if (strcmp(input, "quit") == 0){
                break; 
            }

            /**
             * Repeat Last Command Check:
             * 
             * - Checks if the input string is equal to "!!".
             * - If true and no previous command executed, prints a message indicating no command has been executed yet.
            */
            else{
                if (strcmp(input, "!!") == 0){

                    if (!changed_last){
                        printf("No command has been executed yet\n");
                    }
                    
                    /**
                     * Repeat Last Command:
                     * 
                     * - If the input string is equal to "!!" and a previous command has been executed,
                     *   copies the last executed command to the input buffer.
                    */
                    else
                    {
                        strncpy(input, last_command, strlen(last_command));
                    }
                }

                /**
                 * Conditional Command Execution:
                 * 
                 * - Checks if the input string starts with "if".
                 * - If true, executes the command after "if" and reads subsequent "then" and command.
                 * - If "then" is followed by a valid command, it reads the "else" keyword and the subsequent command.
                 * - Finally, it reads the "fi" keyword, and based on the execution status of the initial command,
                 *   either sets the input buffer to the "then" or "else" command for execution.
                */
                if (strncmp("if", input, 2) == 0){
                    int status_if = exec_command(input+3,1);
                    char then[20],than_command[MAX_COMMAND_LENGTH],else_[20],else_command[MAX_COMMAND_LENGTH],fi[20];
                    fgets(then,20,stdin);
                    then[strlen(then)-1]='\0';

                    /**
                     * Conditional Command Validation:
                     * 
                     * - Checks if the keyword "then" follows the initial "if" condition.
                     * - If true, prints a tab character to indicate the continuation of the command input.
                     * - If false, sets the status to an error code, cleans the input, and continues to the next iteration.
                    */
                    if (strcmp(then, "then") == 0){
                        printf("\t");
                    }
                    else{
                        status = 123456;
                        clean_input();
                        continue;
                    }

                    /**
                     * Conditional Command Input:
                     * 
                     * - Reads input from standard input for the "then" command after the "if" condition.
                     * - Limits the input to a maximum of MAX_COMMAND_LENGTH characters.
                     * - Removes the trailing newline character from the input string.
                     * 
                     * - Reads input from standard input for the "else" keyword.
                     * - Limits the input to a maximum of 20 characters.
                     * - Removes the trailing newline character from the input string.
                    */
                    fgets(than_command,MAX_COMMAND_LENGTH,stdin);
                    than_command[strlen(than_command)-1]='\0';
                    fgets(else_,20,stdin);
                    else_[strlen(else_)-1]='\0';

                    /**
                     * Conditional Keyword Check:
                     * 
                     * - Checks if the input string obtained for the "else" keyword matches "else".
                     * - If the comparison is true, prints a tab character to indicate continuation of the command.
                     * - If the comparison is false, sets the status to an error code, clears the input buffer,
                     *   and continues to the next iteration of the loop.
                    */
                    if (strcmp(else_, "else") == 0){
                        printf("\t");
                    }
                    else{
                        status = 123457;
                        clean_input();
                        continue;
                    }

                    /**
                     * Input Reading:
                     * 
                     * - Reads the command following the "else" keyword from the standard input into the else_command buffer.
                     * - Removes the newline character from the else_command buffer.
                     * - Reads the next keyword "fi" from the standard input into the fi buffer.
                     * - Removes the newline character from the fi buffer.
                    */
                    fgets(else_command,MAX_COMMAND_LENGTH,stdin);
                    else_command[strlen(else_command)-1]='\0';
                    fgets(fi,20,stdin);
                    fi[strlen(fi)-1]='\0';

                    /**
                     * Conditional Check:
                     * 
                     * - Checks if the keyword "fi" follows the completion of the conditional execution block.
                     * - If true, checks the status of the initial command execution.
                     * - If the status is false (zero), copies the then_command to the input buffer for execution.
                     * - If the status is true (non-zero), copies the else_command to the input buffer for execution.
                     * - If "fi" is not found, sets the status to an error code, cleans the input, and continues to the next iteration.
                    */
                    if (strcmp(fi, "fi") == 0){
                        if (!status_if){
                            strncpy(input, than_command, MAX_COMMAND_LENGTH);
                        }
                        else{
                            strncpy(input, else_command, MAX_COMMAND_LENGTH);
                        }
                    }
                    else{
                        status = 123458;
                        clean_input();
                        continue;
                    }
                }

                /**
                 * Command Execution and History Management:
                 * 
                 * - Frees the memory allocated for the command at the current command index in the command history.
                 * - Sets the command at the current command index in the command history to the current input.
                 * - Executes the current command and stores the status.
                 * - Frees the memory allocated for the last command if it was changed.
                 * - Updates the last command to the current input and sets the changed_last flag.
                 * - Increments the number of commands and updates the command index.
                 * - Clears the input buffer for the next command.
                */
                if (commands[num_commands % MAX_COMMANDS]) free(commands[num_commands % MAX_COMMANDS]);

                commands[num_commands % MAX_COMMANDS] = strdup(input);
                status = exec_command(commands[num_commands % MAX_COMMANDS],0);

                if (changed_last) free(last_command);
                last_command = strdup(commands[num_commands % MAX_COMMANDS]);
                changed_last = 1;
                num_commands++;
                command_index = num_commands;

                clean_input();
            }
            continue;
        }

        /**
         * Backspace Key Handling:
         * 
         * - Checks if the Backspace key (ASCII value 127) is pressed.
         * - Verifies if the input buffer has characters to delete.
         * - If characters are present, removes the last character from the input buffer.
         * - Updates the input length accordingly.
         * - Displays the modified command prompt to reflect the changes made.
        */
        else if (c == 127){
            if (input_length > 0){
                input[--input_length] = '\0';
                showCommand(input);
            }
        }

        /**
         * Arrow Key Handling:
         * 
         * - Checks if the Escape key (ASCII value 27) is pressed, indicating a possible arrow key input.
         * - Reads the next character to determine the arrow key direction.
         * - If the next character is '[', proceeds to check for specific arrow key inputs.
         * - If the character following '[' is 'A' (Up arrow key), displays the previous command from history.
         * - If the character following '[' is 'B' (Down arrow key), displays the next command from history.
         * - Handles wrapping around the command history list when necessary.
         * - Updates the input buffer and length accordingly.
         * - Shows the modified command prompt reflecting the retrieved command from history.
        */
        else if (c == 27){
            c = get_char();
            if (c == '['){
                c = get_char();
                if (c == 'A'){
                    if (num_commands >= MAX_COMMANDS || (num_commands < MAX_COMMANDS && command_index > 0)){
                        command_index = mod_func((command_index - 1), MAX_COMMANDS);
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                        showCommand(input);
                    }
                }
                else if (c == 'B'){
                    if (num_commands >= MAX_COMMANDS){
                        command_index = (command_index + 1) % MAX_COMMANDS;
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                        showCommand(input);
                    }
                    else if (num_commands < MAX_COMMANDS && command_index < num_commands){
                            command_index++;
                            if (command_index == num_commands){
                                input[0] = '\0';
                                input_length = 0;
                            }
                            else{
                                strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                                input_length = strlen(input);
                            }
                            showCommand(input);
                    }
                }
            }
        } 
        
        /**
         * Character Input Handling:
         * 
         * - Checks if the entered character is within the printable ASCII range (32 to 126).
         * - Ensures that the input buffer length is within the allowed limit to prevent buffer overflow.
         * - Appends the entered character to the input buffer.
         * - Updates the input buffer length and ensures termination with a null character.
         * - Shows the modified command prompt reflecting the updated input buffer.
        */
        else if (c >= 32 && c <= 126 && input_length < MAX_COMMAND_LENGTH - 1){
            input[input_length++] = c;
            input[input_length] = '\0';
            showCommand(input);
        }
    }

    /**
     * Resource Cleanup and Program Termination:
     * 
     * - Closes the original standard input and output file descriptors.
     * - Frees memory allocated for command history up to a maximum limit.
     * - Frees memory allocated for the prompt title if it has been changed.
     * - Frees memory allocated for the last command if it has been changed.
     * - Frees memory allocated for shell variables.
     * - Returns 0 to indicate successful program termination.
    */
    close(original_stdin);
    close(original_stdout);
    int x = num_commands < MAX_COMMANDS ? num_commands : MAX_COMMANDS;
    for (int i = 0; i < x; i++){
        free(commands[i]);
    }
    if (changed_prompt)free(prompt_title);
    if (changed_last) free(last_command);
    free_variables();
    return 0;
}
