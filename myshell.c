#include "myshell.h"

char *outfile,*last_command = "";
char *prompt_title = "hello";

char ***args,**pipe_commands;

// Command list
char *commands[MAX_COMMANDS]; 

// Input buffer for current command
char input[MAX_COMMAND_LENGTH] = ""; 

int input_length = 0, command_index = 0;
// Current command index
int num_commands = 0; 

int fd, amper,redirect,retid,status,changed_prompt,changed_last,original_stdin,original_stdout;


int main(){
    signal(SIGINT, sigint);

    printf("%s:",prompt_title);

    fflush(stdout);

    //Save orginal stdin and stdout
    original_stdin = dup(0);
    original_stdout = dup(1);

    while (1){

        char c = get_char(); // Read character from terminal

        if (c == '\n'){
            // Enter key pressed
            printf("\n");

            //If press enter without content then continue
            if (input_length == 0){
                clean_input();
                continue;
            }
            if (strcmp(input, "quit") == 0){
                break; // Exit loop and terminate program
            }
            else{
                // Nothing to go back to
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
                        clean_input();
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
                        clean_input();
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
                        clean_input();
                        continue;
                    }
                }

                // Add command to commands list for arrows command 

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
        else if (c == 127){

            // If Backspace key then remove the last character from input buffer
            if (input_length > 0){

                input[--input_length] = '\0';
                // // Print the command
                showCommand(input);
            }
        }
        else if (c == 27){

            // If Esc key, check for arrow keys
            c = get_char();
            if (c == '['){
                c = get_char();
                if (c == 'A'){

                    // If Up arrow key, display the previous command in history
                    if (num_commands >= MAX_COMMANDS || (num_commands < MAX_COMMANDS && command_index > 0)){
                        command_index = mod_func((command_index - 1), MAX_COMMANDS);
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                // // Print the command
                        showCommand(input);
                    }
                }
                else if (c == 'B'){

                    // If Down arrow key, display the next command in history
                    if (num_commands >= MAX_COMMANDS){
                        command_index = (command_index + 1) % MAX_COMMANDS;
                        strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                        input_length = strlen(input);
                // // Print the command
                        showCommand(input);
                    }
                    else if (num_commands < MAX_COMMANDS && command_index < num_commands){
                            command_index++;
                            if (command_index == num_commands){

                                // If at end of command history, clear input buffer
                                input[0] = '\0';
                                input_length = 0;
                            }
                            else{
                                strncpy(input, commands[command_index], MAX_COMMAND_LENGTH - 1);
                                input_length = strlen(input);
                            }
                            // // Print the command
                            showCommand(input);
                    }
                }
            }
        } else if (c >= 32 && c <= 126 && input_length < MAX_COMMAND_LENGTH - 1){

            // If a printable character is pressed then append it to input buffer
            input[input_length++] = c;
            input[input_length] = '\0';
            // Print the command
            showCommand(input);
        }
    }

    close(original_stdin);

    close(original_stdout);

    // Free memory allocated for command history
    int x = num_commands < MAX_COMMANDS ? num_commands : MAX_COMMANDS;

    for (int i = 0; i < x; i++){
        free(commands[i]);
    }

    if (changed_prompt)free(prompt_title);

    if (changed_last) free(last_command);

    // Free the variables hash table memory
    free_variables();


    return 0;
}
