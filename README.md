# Advanced-Programming
Advanced Programming course assignment 1 </br>
# Created By: 
* [Dana Zorohov](https://github.com/danaZo)
* [Noa Nussbaum](https://github.com/Noa-Nussbaum)

MyShell is a shell program developed as part of our Advanced Programming course assignment. The program is designed to interpret and execute user commands in a simulated shell environment. It includes features   commonly found in Unix-like shells,such as command execution, redirection, pipelining, handling variables, and control flow.

#### Getting Started

##### Prerequisites
- GCC compiler
- A Unix-like environment

##### Compilation
To compile the shell program, use the following command in your terminal:
```bash
make
```

##### Running the Shell
To start the shell, run:
```bash
./myshell
```

#### Features and Commands
The shell supports a variety of commands and features as described below:

1. **Command Execution**
   - Execute commands with arguments.
   ```bash
   hello: ls -l
   ```

2. **Background Execution**
   - Execute commands in the background using `&`.
   ```bash
   hello: ls -l &
   ```

3. **Redirection**
   - Redirect output to a file using `>`.
   ```bash
   hello: ls -l > file
   ```
   - Append output to a file using `>>`.
   ```bash
   hello: ls -l >> mylog
   ```
   - Redirect standard error using `2>`.
   ```bash
   hello: ls -l nofile 2> mylog
   ```

4. **Pipelines**
   - Use pipes to direct the output of one command as the input to another.
   ```bash
   hello: cat myfile | grep "search_term"
   ```

5. **Variable Handling**
   - Set and use variables.
   ```bash
   hello: $person = David
   hello: echo $person
   ```

6. **Read Command**
   - Prompt for input and store in a variable.
   ```bash
   hello: read name
   hello: echo $name
   ```

7. **Command History**
   - Navigate through recent commands using the up and down arrow keys.

8. **Control Flow**
   - Implement if-else conditions.
   ```bash
   if date | grep Fri
   then
     echo "Shabat Shalom"
   else
     echo "Hard way to go"
   fi
   ```

9. **Change Prompt**
   - Change the command prompt display.
   ```bash
   hello: prompt = myprompt
   ```

10. **Change Directory**
    - Change the current working directory.
    ```bash
    hello: cd mydir
    ```

11. **Repeat Last Command**
    - Repeat the last executed command using `!!`.
    ```bash
    hello: !!
    ```

12. **Exit**
    - Exit the shell using `quit`.
    ```bash
    hello: quit
    ```

13. **Interrupt Handling**
    - Handle Ctrl-C interrupts gracefully, printing a message rather than terminating.
