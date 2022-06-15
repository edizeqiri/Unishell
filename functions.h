#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

int us_cd(char **args);
int welcome(char **args);
int us_help(char **args);
int us_exit(char **args);
int unimode(int learning);
void pomodoro(int learn_int, int pause_int, char *line);
char *get_ip(char *interface);
char *getusername();

static char *intern_strings[] = {
    "cd",
    "help",
    "exit",
    "welcome",
    "unimode",
    "interface",
    "pomodoro"};

static int (*intern_func[])(char **) = {
    &us_cd,
    &us_help,
    &us_exit,
    &welcome};

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIMS " \t\r\n\a"

#endif /* FUNCTIONS_H_ */