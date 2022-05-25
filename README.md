# Unishell
 The best shell for a CS student

## Installation

1. Get the repo in your preffered way (fork, clone, dowload zip, ...)
2. Compile with `gcc main.c -o Unishell`
3. Start with `./Unishell`

## Functionality

- `cd`, `help` and `exit` are explicit functions. All other things get executed by the OS with the given parameters.

- Arguments only get parsed if they are seperated by whitespace. This means that ``""`` does not chain input but will straight up be ignored.

## TODOs

- [ ] Unimode
    - idea is to make a copy of ``/etc/hosts/`` and then change every "bad" site (e.g. facebook, instagram,...) to ``127.0.0.1`` so that user can't access it.
- [ ] config file
- [ ] installation process (start a script which generates all the needed things for Unimode)
    - config file
    - ...
- [ ] Minigames and 


