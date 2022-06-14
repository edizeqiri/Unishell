# Unishell


<p align="center"><img src="https://user-images.githubusercontent.com/89740646/173557119-a9a465e4-bcfd-4162-82f2-f0c630c03a3f.png">


## Installation

### Makefile

1. Compile: `make`
2. Compile and run: `make exe`
3. Clean: `make clean` 

### Vanilla
1. Get the repo in your preffered way (fork, clone, dowload zip, ...)
2. Compile with `gcc main.c -o Unishell`
3. Start with `./Unishell`

## Functionality

- `cd`: Change current Directory
- `help`: Helper function to show functionalities
- `exit`: Exit shell
- `welcome`: Displays the welcome message
- `Unimode`: Start your learning journey here!
- `Pomodoro`: Starts a Pomodoro timer.
- `interface [ip_interface]`: Change which interface to use.

All other things get executed by the OS with the given parameters.
Arguments only get parsed if they are seperated by whitespace. This means that ``""`` does not chain input but will straight up be ignored.

## TODOs

- [x] Unimode
    - idea is to make a copy of ``/etc/hosts/`` and then change every "bad" site (e.g. facebook, instagram,...) to ``127.0.0.1`` so that user can't access it.
- [x] Pomodoro
- [x] UI: vertical bar is hardcoded -> needs to be alined.
- [x] UI: add IP

