#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

#include "functions.h"

int learning = 0;
int learn_int = 25;
int pause_int = 5;
int intervals = 6;
int time_us = 0;

char *intern_strings[] = {
    "cd",
    "help",
    "exit"};

int (*intern_func[])(char **) = {
    &us_cd,
    &us_help,
    &us_exit};

int size_intern_strings()
{
  return sizeof(intern_strings) / sizeof(char *);
}

/**
 * @brief Store input into a buffer which then gets saved into a String
 *
 * @return char* input line
 */
char *read_input()
{
  char *line = NULL;
  ssize_t size = 0;

  if (getline(&line, &size, stdin) == -1)
  {

    // Look for EOF (End of File)
    if (feof(stdin))
    {
      exit(EXIT_SUCCESS);
    }

    // Something went wrong
    else
    {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

// us_helper function
void error_exiter(char *trigger)
{
  if (!trigger)
  {
    fprintf(stderr, "What did you do?!\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Splits input from the whole string into "words" and saves it into string array
 * e.g "I love honey" => ["I", "love", "honey"]
 *
 * @param line input string
 * @return char** string array containing words
 */
char **split_input(char *line)
{

  int bufsize = TOKEN_BUFFSIZE, index = 0;

  // String array
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  error_exiter(token);

  // Take whole String and partialize it into tokens which will then be processed and accessed by a pointer
  token = strtok(line, TOKEN_DELIMS);

  // Put tokens into string array one by one
  while (token != NULL)
  {
    tokens[index] = token;
    index++;

    // If buffer to small double its size and reallocate
    if (index >= bufsize)
    {
      bufsize += TOKEN_BUFFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      error_exiter(token);
    }

    token = strtok(NULL, TOKEN_DELIMS);
  }
  tokens[index] = NULL;
  return tokens;
}

/**
 * @brief Executes given commands and returns if it worked.
 *
 * @param args string array from split_input
 * @return int if failure or success
 */
int execute(char **args)
{

  if (args[0] == NULL)
  {
    // An empty command was entered.
    return 1;
  }

  if (strcmp(args[0], "14") == 0)
  {
    printf("( ͡ಠ ʖ̯ ͡ಠ ) Imagine giving students an exercise sheet with only 14 pages and not 21. \n \n");
    return 1;
  }

  if (strcmp(args[0], "Unimode") == 0 || strcmp(args[0], "unimode") == 0)
  {
    learning = unimode(learning);
    return 1;
  }

  if (strcmp(args[0], "Pomodoro") == 0 || strcmp(args[0], "pomodoro") == 0)
  {
    if (args[1] != NULL && strcmp(args[1], "-h") == 0)
    {
      printf("Usage ☜(⌒▽⌒)☞: Pomodoro [Learning Interval] [Pause Interval]\n");
      return 1;
    }
    else if (args[1] != NULL && strcmp(args[1], "stop") == 0)
    {
      intervals = 0;
    }

    else if (args[1] != NULL && args[2] != NULL)
    {
      intervals = 6;
      if (scanf("%d", args[1]) && scanf("%d", args[2]))
      {
        pomodoro(atoi(args[1]), atoi(args[2]));
        return 1;
      }
    }
    else
    {
      printf("\nStarting Pomodoro with 25min learn and 5min pause interval ☜(⌒▽⌒)☞\n \n");
      pomodoro(learn_int, pause_int);
      return 1;
    }
  }

  for (int i = 0; i < size_intern_strings(); i++)
  {
    if (strcmp(args[0], intern_strings[i]) == 0)
    {
      return (*intern_func[i])(args);
    }
  }

  pid_t pid, wait_pid;
  int status;

  pid = fork();

  // if child
  if (pid == 0)
  {

    // v for vector and p to let OS find program and not give full path
    if (execvp(args[0], args) == -1)
    {
      perror("Unishell ( ͡ಠ ʖ̯ ͡ಠ )");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid < 0)
  {
    // Error forking
    perror("Unishell （　ﾟДﾟ）");
  }
  else
  {

    // if parent
    do
    {
      // wait for child to die
      wait_pid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


/**
 * @brief Main loop of the shell which does 3 things.
 * 1. Get input and store it into a string
 * 2. Parse the input
 * 3. Execute the input
 */
void main_loop()
{
  char *line;
  char **args;
  int status;

  // get pc name and username
  char cwd[PATH_MAX];
  char hostname[HOST_NAME_MAX];

  gethostname(hostname, HOST_NAME_MAX);

  us_help(NULL);

  // While status is good (0) read-split into args-execute
  do
  {
    getcwd(cwd, sizeof(cwd));
    printf(GRN "(" YEL "%s" CYN "@" RED "%s" GRN ")┬" GRN "[" CYN "%s" GRN "]" GRN "\n            └─" CYN "₿ ", hostname, getusername(), cwd);
    line = read_input();
    args = split_input(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  main_loop();

  printf("See you later alligator!");
  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
