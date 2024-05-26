#ifndef MYSHELL_HPP
#define MYSHELL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <cstdlib>
#include <sys/ioctl.h>
#include <iomanip>
#include <map>
#include <algorithm> 
#include <iterator>
#include <fcntl.h>
#include <sys/stat.h> 
#include <termios.h>

class myShell {
private:
    static std::string currPrompt;               // The prompt before each command
    int prevStatus;                          // Stores the status number of the last command
    std::string prevCommand;                 // Stores the last command
    static volatile sig_atomic_t childPid;   // Stores the child process ID
    static void sigintHandler(int);          // Handler for ctrl+C
    std::map<std::string, std::string> variables; // Dictionary to store each $ variable in key-value pattern 

public:
    myShell();                               // Constructor
    ~myShell() = default;                    // Destructor

    static void setPrompt(const std::string&); // Changes the name of the prompt
    void run();                                // Runs the shell
    static volatile sig_atomic_t& getChildPid(); // Static member function to get childPid
    void executeCommand(const std::string&, bool, bool);
    void executePipes(std::vector<std::string>, bool);
    int ifCaseHandeling(std::string);
};

#endif // MYSHELL_HPP
