#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 1024


void main_loop();

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  main_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

void main_loop(void)
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
