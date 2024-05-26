# Advanced-Programming
Advanced Programming course assignment 1

# MyShell


MyShell is a straightforward shell implementation crafted in C++, offering essential command-line interface functionalities. It includes features commonly found in Unix-like shells, such as command execution, input/output redirection, background processes, and basic control flow structures like if statements.

## Features

- **Command Execution:** Run single commands or command pipelines with input/output redirection.
- **Built-in Commands:** Supports built-in commands like `cd`, `echo`, and `read`.
- **Variable Management:** Modify the prompt and define custom variables.
- **Signal Handling:** Manage Ctrl+C interruptions smoothly.
- **Control Flow:** Use `if` statements for conditional execution.
- **Background Execution:** Execute commands in the background with the `&` operator.
- **Error Handling:** Includes basic error handling for syntax errors in `if` statements.

## Getting Started

### Prerequisites

- C++ compiler with C++2a support (e.g., Clang++-14).
- Use WSL on Windows

### Compilation

Compile the code using a C++ compiler and the Makefile for easy running. For example:

```bash
make run
```

## Running the Shell

Run the compiled executable:

```bash
./myshell
```

## Commands Guide

- **`>`**: Redirects standard output (stdout) to a specified file. Example: `command > output.txt` redirects stdout to `output.txt`.
- **`2>`**: Redirects standard error (stderr) to a specified file. Example: `command 2> error.txt` redirects stderr to `error.txt`.
- **`>>`**: Appends output to a file, creating it if it doesn't exist. Example: `echo "text" >> output.txt` appends "text" to `output.txt`.
- **`prompt`**: Changes the prompt text. Usage: `prompt = "new_prompt"`.
- **`echo`**: Prints arguments to the terminal. Example: `echo Hello, World!` displays "Hello, World!".
- **`$?`**: Shows the exit status of the last command. Example: `echo $?` prints the last command's exit status.
- **`!!`**: Repeats the last command. Typing `!!` reruns the previous command.
- **`cd`**: Changes the current directory. Example: `cd /path/to/directory`.
- **`quit`**: Exits the shell. Typing `quit` terminates the session.
- **`Up Arrow`**: Scrolls up through previously entered commands. Pressing the up arrow key will show the last command executed.
- **`Down Arrow`**: Scrolls down through the command history. If you have navigated up through previous commands, pressing the down arrow key will take you to more recent commands.
- **`Left Arrow`**: Moves the cursor to the left within the current command line. This allows you to edit the command before executing it.
- **`Right Arrow`**: Moves the cursor to the right within the current command line. This is useful for navigating and editing commands before execution.
- **`^c`**: Interrupts a running process with Ctrl+C. Displays "You Typed Control-C!" but keeps the shell running.
- **`pipe`**: Connects commands, passing the output of one as input to another. Example: `command1 | command2`.
- **`$var`**: Creates and uses variables. Example: `$name = "John"` assigns "John" to `$name`.
- **`read`**: Prompts for and stores user input in variables. Example: `read variable_name`.
- **`echo $var`**: Prints the value of a variable. Example: `echo $name` displays "John".
- **`if/else`**: Implements conditional execution. Example:

  ```bash
  if [ condition ]
  then
      [ command if true ]
  else
      [ command if false ]
  fi
  ```


