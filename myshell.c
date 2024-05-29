#include "myshell.h"

char *outfile,*last_command = "", *prompt_name = "hello";
char ***args,**pipe_commands, *commands[MAX_COMMANDS]; // Array to store command history
char input[MAX_COMMAND_LENGTH] = ""; // Input buffer for current command
int input_length = 0, command_index = 0,num_commands = 0; // Index of currently displayed command
int fd, amper,redirect,retid,status,changed_prompt,changed_last,orig_stdin,orig_stdout;


int main(){
    signal(SIGINT, sigint_handler);
    printf("%s:",prompt_name);
    fflush(stdout);
    //save orginal stdin, stdout
    orig_stdin = dup(0);
    orig_stdout = dup(1);
    while (1){
        char c = getch(); // Read a character from terminal

        if (c == '\n'){
            // Enter key pressed, execute command
            printf("\n");
            //if press enter without content
            if (input_length == 0){
                cleanInput();
                continue;
            }
            if (strcmp(input, "quit") == 0){
                break; // Exit the loop and terminate the program
            }
            else{
                if (strcmp(input, "!!") == 0){
                    if (!changed_last){
                        printf("No command has been executed yet\n");
                    }
                    else{
                        strncpy(input, last_command, strlen(last_command));
                    }
                }
                if (strncmp("if", input, 2) == 0){
                    int status_if = exec_command(input+3,1);
                    char then[20],than_command[MAX_COMMAND_LENGTH],else_[20],else_command[MAX_COMMAND_LENGTH],fi[20];
                    fgets(then,20,stdin);
                    then[strlen(then)-1]='\0';
                    if (strcmp(then, "then") == 0){
                        printf("\t");
                    }
                    else{
                        status = 158461;
                        cleanInput();
                        continue;
                    }
                    fgets(than_command,MAX_COMMAND_LENGTH,stdin);
                    than_command[strlen(than_command)-1]='\0';
                    fgets(else_,20,stdin);
                    else_[strlen(else_)-1]='\0';
                    if (strcmp(else_, "else") == 0){
                        printf("\t");
                    }
                    else{
                        status = 158462;
                        cleanInput();
                        continue;
                    }
                    fgets(else_command,MAX_COMMAND_LENGTH,stdin);
                    else_command[strlen(else_command)-1]='\0';
                    fgets(fi,20,stdin);
                    fi[strlen(fi)-1]='\0';
                    if (strcmp(fi, "fi") == 0){
                        if (!status_if){
                            strncpy(input, than_command, MAX_COMMAND_LENGTH);
                        }
                        else{
                            strncpy(input, else_command, MAX_COMMAND_LENGTH);
                        }
                    }
                    else{
                        status = 158463;
                        cleanInput();
                        continue;
                    }
                }
                // Add command to command history
                if (commands[num_commands % MAX_COMMANDS]) free(commands[num_commands % MAX_COMMANDS]);
                commands[num_commands % MAX_COMMANDS] = strdup(input);
                status = exec_command(commands[num_commands % MAX_COMMANDS],0);
                if (changed_last) free(last_command);
                last_command = strdup(commands[num_commands % MAX_COMMANDS]);
                changed_last = 1;
                num_commands++;
                command_index = num_commands;
                cleanInput();
            }
            continue;
        }
        else if (c == 127){
            // If Backspace key is pressed, remove the last character from input buffer
            if (input_length > 0){
                input[--input_length] = '\0';
                display_command(input);
            }
        }
        else if (c == 27){
            // If Esc key is pressed, check for arrow keys
            c = getch();
            if (c == '['){
                c = getch();
                if (c == 'A'){
                    // If Up arrow key is pressed, display the previous command in history
                    if (num_commands >= MAX_COMMANDS || (num_commands < MAX_COMMANDS && command_index > 0)){
                        command_index = mod((command_index - 1), MAX_COMMANDS);
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                        display_command(input);
                    }
                }
                else if (c == 'B'){
                    // If Down arrow key is pressed, display the next command in history
                    if (num_commands >= MAX_COMMANDS){
                        command_index = (command_index + 1) % MAX_COMMANDS;
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                        display_command(input);
                    }
                    else if (num_commands < MAX_COMMANDS && command_index < num_commands){
                            command_index++;
                            if (command_index == num_commands){
                                // If at the end of command history, clear the input buffer
                                input[0] = '\0';
                                input_length = 0;
                            }
                            else{
                                strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                                input_length = strlen(input);
                            }
                            display_command(input);
                    }
                }
            }
        } else if (c >= 32 && c <= 126 && input_length < MAX_COMMAND_LENGTH - 1){
            // If a printable character is pressed, append it to input buffer
            input[input_length++] = c;
            input[input_length] = '\0';
            display_command(input);
        }
    }
    close(orig_stdin);
    close(orig_stdout);
    // Free memory allocated for command history
    int x = num_commands < MAX_COMMANDS ? num_commands : MAX_COMMANDS;
    for (int i = 0; i < x; i++){
        free(commands[i]);
    }
    if (changed_prompt)free(prompt_name);
    if (changed_last) free(last_command);
    free_hash_table();
    return 0;
}
