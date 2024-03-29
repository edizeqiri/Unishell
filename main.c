#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <ifaddrs.h>
#include <termios.h>

#include "functions.h"

int learning = 0;
int learn_int = 25;
int pause_int = 5;
int intervals = 6;
int time_us = 0;
char *ip;
char *line;

/**
 * @brief Read character from stdin without buffering.
 *
 * This function reads a character from stdin without buffering.
 * It is used to read the input and save the line in a custom buffer (line).
 * This is done to be able to keep what was printed after the pomodoro thread
 * cleans the console line.
 *
 * It works by disabling canonical and echo mode. We then have to to print the
 * read carachter to stdout manually.
 *
 * @return char character read from stdin without waiting for a newline.
 */
char getch()
{
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

/**
 * @brief Reads every character pressed with help of getch() and stores it in a buffer.
 *
 * Line buffer (1024 chars) is allocated at the start. On every key press the output
 * stream stdout is flushed to ensure that it displays correctly. When pressing backspace
 * the line to be adjusted to the new size. It does that by adding the \0 delimiter.
 * After reading a line break, a line break is added to the buffer and the stdout is
 * flushed.
 *
 * @return char* input line
 */
char *read_input()
{
  char *line = NULL;
  int bufsize = 1024;
  ssize_t size = 0;
  line = malloc(sizeof(char) * bufsize);

  while (1)
  {
    fflush(stdout);
    char c = getch();
    if (c == '\n')
    {
      break;
    }
    else if (c == 127) // Backspace
    {
      if (size > 0)
      {
        printf("\b \b");
        size--;
        // line = realloc(line, size);
        line[size] = '\0';
      }
    }
    else
    {
      printf("%c", c);
      // line = realloc(line, size + 1);
      line[size] = c;
      size++;
    }
  }
  line[size] = '\n';
  line[size + 1] = '\0';
  printf("\n");
  fflush(stdout);
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

  // IP Adress
  if (strcmp(args[0], "interface") == 0 && args[1] != NULL)
  {
    if (strcmp(args[1], "tun0") == 0)
    {
      strcpy(ip, "tun0");
    }
    else if (strcmp(args[1], "wlan0") == 0)
    {
      strcpy(ip, "wlan0");
    }
    else if (strcmp(args[1], "lo") == 0)
    {
      strcpy(ip, "lo");
    }
    else if (strcmp(args[1], "eth0") == 0)
    {
      strcpy(ip, "eth0");
    }
    else
    {
      fprintf(stderr, "Unknown interface.\n");
      return 1;
    }
    return 1;
  }

  // gimmick
  if (strcmp(args[0], "14") == 0)
  {
    printf("( ͡ಠ ʖ̯ ͡ಠ ) Imagine giving students an exercise sheet with only 14 pages and not 21. \n \n");
    return 1;
  }

  // Unimode
  if (strcmp(args[0], "Unimode") == 0 || strcmp(args[0], "unimode") == 0)
  {
    learning = unimode(learning);
    return 1;
  }

  // Pomodoro
  if (strcmp(args[0], "Pomodoro") == 0 || strcmp(args[0], "pomodoro") == 0)
  {
    if (args[1] != NULL && strcmp(args[1], "-h") == 0 || args[1] != NULL && strcmp(args[1], "-help") == 0)
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
      pomodoro(atoi(args[1]), atoi(args[2]), line, ip);
      return 1;
    }
    else
    {
      pomodoro(learn_int, pause_int, line, ip);
      return 1;
    }
  }

  // Check if command is intern
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
  char **args;
  int status;

  // get pc name and username
  char cwd[PATH_MAX];
  char hostname[HOST_NAME_MAX];

  gethostname(hostname, HOST_NAME_MAX);

  // get length of username and hostname
  int hostname_len = strlen(hostname);
  int username_len = strlen(getusername());
  welcome(NULL);

  // While status is good (0) read-split into args-execute
  do
  {
    getcwd(cwd, sizeof(cwd));
    printf(GRN "(" YEL "%s" CYN "@" RED "%s" GRN ")┬" GRN "[" CYN "%s" GRN "]―" GRN "[" CYN "%s" GRN "]"
               "\n%*c└─" CYN "₿ ",
           hostname, getusername(), get_ip(ip), cwd, (hostname_len + username_len + 3), ' ');
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
  ip = malloc(sizeof(char) * 6);
  strcpy(ip, "eth0");

  // Run command loop.
  main_loop();

  printf("See you later alligator!\n");
  // Perform any shutdown/cleanup.
  free(ip);
  return EXIT_SUCCESS;
}
