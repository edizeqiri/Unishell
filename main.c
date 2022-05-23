#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIMS " \t\r\n\a"

int main(int argc, char **argv);
void main_loop();
char *read_input();
void error_exiter(char *trigger);
char **split_input(char *line);
int execute(char **args);

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  main_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
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

  // While status is good (0) read-split into args-execute
  do {
    printf("=> ");
    line = read_input();
    args = split_input(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
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

  if (getline(&line, &size, stdin) == -1){

    // Look for EOF (End of File)
    if (feof(stdin)) {
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

// helper function
void error_exiter(char *trigger) {
if (!trigger) {
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
char **split_input(char *line) {

  int bufsize = TOKEN_BUFFSIZE, index = 0;

  // String array
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  error_exiter(token);

  // Take whole String and partialize it into tokens which will then be processed and accessed by a pointer
  token = strtok(line, TOKEN_DELIMS);

  // Put tokens into string array one by one
  while (token != NULL) {
    tokens[index] = token;
    index++;

    // If buffer to small double its size and reallocate
    if (index >= bufsize) {
      bufsize += TOKEN_BUFFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
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
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}
