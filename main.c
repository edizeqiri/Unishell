#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIMS " \t\r\n\a"

int us_cd(char **args);
int us_help(char **args);
int us_exit(char **args);

char *intern_strings[] = {
  "cd",
  "help",
  "exit"
};

int (*intern_func[]) (char **) = {
  &us_cd,
  &us_help,
  &us_exit
};

int size_intern_strings() {
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

// us_helper function
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
int execute(char **args) {

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (int i = 0; i < size_intern_strings(); i++) {
    if (strcmp(args[0], intern_strings[i]) == 0) {
      return (*intern_func[i])(args);
    }
  }

  pid_t pid, wait_pid;
  int status;

  pid = fork();

  // if child
  if (pid == 0) {

    // v for vector and p to let OS find program and not give full path
    if (execvp(args[0], args) == -1) {
      perror("Unishell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("Unishell");
  } else {
    
    // if parent
    do {
      // wait for child to die
      wait_pid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
 * @brief change directory
 * 
 * @param args 
 * @return int 
 */
int us_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Unishell: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("Unishell");
    }
  }
  return 1;
}

/**
 * @brief exit the shell duh
 * 
 * @param args for convinience
 * @return int 
 */
int us_exit(char **args)
{
  return 0;
}

int us_help(char **args)
{
  int i;
  printf("\n==============================================================\n");
  printf(" /$$   /$$           /$$           /$$                 /$$ /$$\n\
| $$  | $$          |__/          | $$                | $$| $$\n\
| $$  | $$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$\n\
| $$  | $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$\n\
| $$  | $$| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$\n\
| $$  | $$| $$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$| $$\n\
|  $$$$$$/| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$\n\
 \\______/ |__/  |__/|__/|_______/ |__/  |__/ \\_______/|__/|__/\n");
 printf("==============================================================\n");
  printf("\n|=========================================================================|\n");
  printf("|                   Unishell from students for students                   |\n");
  printf("|  Our improved shell will make you feel like learning could never stop!  |\n");
  printf("|                     The following are built in:                         |\n");
  printf("|=========================================================================|\n \n");
  

  for (i = 0; i < size_intern_strings(); i++) {
    printf("|%d.  %s\n",i+1 , intern_strings[i]);
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

  us_help(NULL);
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

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  main_loop();



  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
