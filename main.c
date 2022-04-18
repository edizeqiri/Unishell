#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 1024


void main_loop();
char *read_input();

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  main_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

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