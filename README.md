# Betty : A Custom Unix Shell in C

# Introduction

**Betty** is a custom Unix shell written in C that provides basic CLI (command-line-interface) functionalities. This shell is designed to handle multiple commands, execute them in the background, and support various built-in and external commands.

### How to run?

To compile and run this shell, you need:

- **GCC**: The GNU Compiler Collection to compile the C files.
- **Make**: The build automation tool to simplify the build process.
- **Unix/Linux Environment**: The shell is designed to run in a Unix-like environment.

1. **Clone the repository**

   ```   gh repo clone OSN-Monsoon-2024/mini-project-1-amoiba42
   cd myshell
   ```

2. **Compile the shell**

   Use

   `bash  make `

   and then
   `bash ./a.out `

# Files

- **main.c**: The main driver of the shell. Initializes the shell and handles the command loop.

- **prompt.c**: Manages the display of the shell prompt.

- **setup.c**: Handles initial shell configuration and setup.

- **headers.h**: Contains necessary function declarations and macros.

- **command.c**: Parses and executes user commands.

- **hop.c**: Contains functions to handle directory navigation.

- **log.c**: Manages the logging of command history.

- **proclore.c**: Contains process management functions.

- **reveal.c**: Displays additional system information.

- **seek.c**: Implements search functionality within the shell.

### Supported Commands

1. **hop**
   The hop command changes the current working directory of the shell and prints the full path of the new directory after the change. It supports directory paths as arguments and provides several useful flags for directory navigation.

### Supported Flags

- ` . `
Retains the current directory and prints its full path.

- ` .. `
Moves to the parent directory of the current directory.

- ` ~ `
Changes the directory to the user's home directory.

- ` - `
Switches to the last visited directory.

- ` `
If any flag is not specified,the command changes the directory to the user's home directory

- ` </path> `
Changes the directory to the specified path

2. **reveal**
   The reveal command lists all the files and directories in the specified directories in lexicographic order (default reveal does not show hidden files).It supports both relative and absolute paths.

### Supported Flags

   Besides supporting the flags supported by hop function , reveal also supports

   - ```-l``` : displays extra information

   - ```-a``` : displays all files, including hidden files 


3. **log**

   Stores the history of the latest 15 commands given to shell

### Supported Functions

- ```purge``` : Clears all the log currently stored

- ```execute <command_no.>``` : Execute the command at position in log (ordered from most recent to oldest)

4. **seek** 

   The ‘seek’ command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

### Supported Flags

-  ```-d``` : Only look for directories (ignore files even if name matches)

-  ```-f``` : Only look for files (ignore directories even if name matches)

-   ```-e```: This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect.



5. **proclore**

   The proclore command details information about processes on a system. It provides crucial process details and, when used without arguments, displays information about the shell itself.

   When you run proclore with a specific PID, it outputs the following details:

   1. **PID** : The Process ID of the target process.
   2. **Process Status** : The current status of the process.    
                   Possible values include:
                     R / R+ : Running
                     S / S+ : Sleeping
                     Z : Zombie
   3.  **Process Group** : The process group ID associated with the process.
   4.  **Virtual Memory** : The total virtual memory allocated for the process.
   5.  **Executable Path** : The full path to the executable file of the process.

# Assumptions

1. All paths,filenames,names or any strings in general are assumed to be a max of 4096 characters

2. Suppose the output is to be printed in a file but the command gets an error then the error is being printed to shell itself

3. Consecutive duplicate commands won't be logged ,however any extra whitespaces,tabs or extra symbols would mean that both the commands get logged separately

4. Log file will store a maximum of 15 latest commands at a time

5. Just like ordinary bash,any whitespaces,tabs etc. or invalid characters between flags will give errors.Be careful while giving inputs

6. Background processes are initiated by putting '&' at the end of each command,
   
Background processes will not execute any of the specially executed commands like `hop , seek , reveal , proclore log etc. ` .

7. Executing sleep consecutively for more than 2s will print the total time 

8. `reveal  ` will list all files (white) , directories(blue) and executables (green) in the terminal. However using any flag with reveal which involves revealing all contents of another part won't turn the executables green

9. It might occasionally take time for the background processes table to be updated when its processes states have been changed or some state has been brought from foreground to background,however you can check these cases by pinging such process with some signal and you would mostly find that the process does not exist or some similar behavior that indicates the table is not updated in live time because of delay in sending signals

10. Ctrl+D command should ideally be given as input whenever a new prompt is printed

11. This shell is not executing background processes inside pipes,however placing an & at the end of the pipe would make the pipe run in the background as a whole 

12. If two pipes are placed simulataneously without any non-empty string in between,then error would be printed

13. In general vim command was producing errors for me when  pinged or ran in background,please avoid using that.

14. Due to double forking there may be instances when command is being printed twice,please ignore this

15. Output for custom commands have mostly been separated by '--\n' to distinguish between them

16. You are encouraged to look at the .myshrc file and use the given aliases to improve workflow and efficiency

17. The background processes won't complete custom executed commands,be cautitious when making a background pipe with multiple commands or while using Ctrl+Z for these commands

18. In general all subdirectories or log files can have a maximum of 4096 entries or strings of length 4096

19. Every command is being stored as it is without checking if it is erroneous or not in log file

20. For some processes, linux does not let us obtain paths to their executables due to permission issues,such paths are not being printed

21. Stick to the flags mentioned in this README file for custom executed commands otherwise you will get an error


# Acknowlegments

I would like to acknowledge the use of OpenAI's ChatGPT-4 ,Linux Manpages and Man7.org for creating this shell and debugging any errors
