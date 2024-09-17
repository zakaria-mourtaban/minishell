# Minishell

## Introduction

Minishell is a simple shell implementation project. It aims to provide a basic command-line interface similar to Bash, where users can execute commands, handle file redirection, pipelines, environment variables, and manage signals.

## Features

The mandatory part of this project implements the following core functionalities:

1. **Prompt**: Displays a prompt when waiting for a new command.
2. **Command History**: Maintains a history of previously executed commands.
3. **Executable Search**: Finds and executes commands based on the `PATH` environment variable, or using relative/absolute paths.
4. **Global Variables**: Minimizes the use of global variables, with one allowed for signaling.
5. **Quotes Handling**:
   - Single quotes (`'`) prevent interpretation of special characters.
   - Double quotes (`"`) prevent interpretation of special characters, except for `$`.
6. **Redirections**:
   - `<`: Redirect input.
   - `>`: Redirect output.
   - `<<`: Implements here-document functionality.
   - `>>`: Redirect output in append mode.
7. **Pipes (`|`)**: Supports piping where the output of one command is used as the input to the next.
8. **Environment Variables**: Handles `$` for variable expansion and `$$` for the exit status of the most recently executed foreground pipeline.
9. **Signal Handling**:
   - `Ctrl-C`: Interrupts and shows a new prompt.
   - `Ctrl-D`: Exits the shell.
   - `Ctrl-\`: Does nothing.
10. **Built-in Commands**:
    - `echo` with the `-n` option.
    - `cd` with relative or absolute paths.
    - `pwd` to print the working directory.
    - `export` for managing environment variables.
    - `unset` for removing environment variables.
    - `env` to display the current environment variables.
    - `exit` to close the shell.

## Requirements

- **C Language**: The project is written in C and adheres to the 42 Norm.
- **Memory Management**: All allocated memory is freed when necessary; no leaks are tolerated except those from the `readline()` function.
- **Makefile**: A Makefile is included to compile the project using the flags `-Wall -Wextra -Werror` without relinking. The rules include:
  - `all`: Compiles the program.
  - `clean`: Removes object files.
  - `fclean`: Removes all generated files.
  - `re`: Rebuilds the project.

## Usage

To compile the project:

```bash
make
```

To run the shell:

```bash
./minishell
```

## Reference

- [Bash Manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/)

## Know git better :) ->

git checkout main<br>
git pull origin main<br>
git checkout -b newbranchname<br>
git add *<br>
git commit -m "pushmessage"<br>
git push orgin newbranchname<br>
to remove old local branches<br>
git fetch --prune && git branch -d $(git branch --merged | grep -v 'main')<br>

## Ignore realine leaks
Take a copy of the readline.supp file put it in your minishell and run the next command -><br>
<br>
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell<br>
