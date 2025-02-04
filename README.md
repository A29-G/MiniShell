# MiniShell
This C code snippet implements a simple command-line interface (CLI) shell named "minishell" with functionalities to execute both internal and external commands. Here's a breakdown of the key components and functions:

### Overview:
- The code defines various functions and global variables to handle command execution.
- The `main()` function initializes the shell by clearing the screen and calling `scan_input()` to process user commands.

### Key Components:

1. **Global Variables**:
    - `char prompt[100] = "minishell$";` - The shell prompt displayed to the user.
    - `int pid = -1;` - Stores the process ID of the current child process.
    - `int status;` - Stores the status of the child process.
    - `FILE *fptr;` - File pointer used to handle file operations.
    - `char *external[200];` - Array of strings to store external commands.

2. **Function `my_handler()`**:
    - **Purpose**: Signal handler function to handle interrupt signals (SIGINT and SIGTSTP).
    - **Behavior**: Prints the shell prompt when an interrupt signal is received and no child process is running.

3. **Function `main()`**:
    - **Purpose**: Initializes the shell and starts processing user input.
    - **Behavior**: Clears the screen and calls `scan_input()` with the prompt and input string as arguments.

4. **Function `get_command(char *input_string)`**:
    - **Purpose**: Extracts the first command from the input string.
    - **Behavior**: Removes leading spaces, identifies the first word in the input string, and returns it as a dynamically allocated string.

5. **Function `scan_input(char *prompt, char *input_string)`**:
    - **Purpose**: Processes user input in a loop, handling both internal and external commands.
    - **Behavior**: Displays the prompt, reads user input, and determines the command type (internal or external). Executes the command accordingly.

6. **Function `check_command_type(char *command)`**:
    - **Purpose**: Determines if the given command is an internal or external command.
    - **Behavior**: Checks the command against a list of internal and external commands and returns the appropriate type.

7. **Function `extract_external_commands(char **external_commands)`**:
    - **Purpose**: Extracts external commands from a file and stores them in an array.
    - **Behavior**: Reads commands from "program.txt" and stores them in the `external_commands` array.

8. **Function `execute_external_commands(char *input_string)`**:
    - **Purpose**: Executes external commands using the `execvp()` system call.
    - **Behavior**: Converts the input string into an array of arguments, handles pipes if present, and executes the command.

9. **Function `execute_internal_commands(char *input_string)`**:
    - **Purpose**: Executes internal commands like `exit`, `pwd`, `cd`, and `echo`.
    - **Behavior**: Executes the appropriate internal command based on the input string.

This code provides a basic implementation of a shell that can handle both internal and external commands, manage signals, and process user input in a loop. The shell supports command execution, directory navigation, and basic system interaction through internal commands.
