#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 1024


void main_loop();
char *read_input(void);

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

char *read_input(void)
{
  int size = BUFFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * size);

  // Needs to be an int to check for EOF
  int character;

  if (!buffer) {
    fprintf(stderr, "unishell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    character = getchar();

    // If we hit EOF (End of File, like ctrl+D), replace it with a null character and return.
    if (character == EOF || character == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = character;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= size) {
      size += BUFFSIZE;
      buffer = realloc(buffer, size);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}