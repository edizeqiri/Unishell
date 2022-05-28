#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>

#include "functions.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


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
      perror("Unishell (╬ ಠ益ಠ)");
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

/**
 * @brief gets username of OS user
 * 
 * @return char* the username
 */
char *getusername() {
  char *name; 
  struct passwd *pass; 
  pass = getpwuid(getuid()); 
  name = pass->pw_name;
  return name;
}

int us_help(char **args)
{

  int i;
  printf(GRN"\n==============================================================\n"CYN);
  printf(" /$$   /$$           /$$           /$$                 /$$ /$$\n\
| $$  | $$          |__/          | $$                | $$| $$\n\
| $$  | $$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$\n\
| $$  | $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$\n\
| $$  | $$| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$\n\
| $$  | $$| $$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$| $$\n\
|  $$$$$$/| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$\n\
 \\______/ |__/  |__/|__/|_______/ |__/  |__/ \\_______/|__/|__/\n"GRN);
 printf("==============================================================\n");
  printf(GRN"\n|=========================================================================|\n");
  printf("|"CYN"                 The Unishell for students from students                 "GRN"|\n");
  printf("|"CYN"  Our improved shell will make you feel like learning could never stop!  "GRN"|\n");
  printf("|"CYN"                     Happy learning "RED"( ˘ ³˘)♥"GRN"                             "GRN"|\n", getusername());
  printf("|=========================================================================|\n \n");
  
  return 1;
}


int unimode(learning)
{

  // if unimode off
  if (learning == 0)
  {
    // /etc/hosts
    // make backup of hosts file hosts.old
    if (rename("/etc/hosts", "/etc/hosts.old") != 0)
    {
      perror("Unishell ಥ_ಥ");
      return -1;
    }

    // put in our host file
    if (rename("hosts", "/etc/hosts") != 0)
    {
      perror("Unishell ಥ_ಥ");
      return -1;
    }
    printf("You have started the ZEN ᕙ(⇀‸↼)ᕗ\n");
    printf("To leave The ZEN ᕙ(⇀‸↼)ᕗ just type the command 'Unishell' again.\n");
  } // if unimode on
  else
  {

    // revert changes
    if (remove("/etc/hosts") != 0)
    {
      perror("Unishell ಥ_ಥ");
      return -1;
    }

    if (rename("/etc/hosts.old", "/etc/hosts") != 0)
    {
      perror("Unishell ಥ_ಥ");
      return -1;
    }

    printf("You have left The ZEN ᕙ(⇀‸↼)ᕗ\n");
    printf("To start The ZEN ᕙ(⇀‸↼)ᕗ again just type the command 'Unishell'.\n");
  }

  return 1;
}

void pomodoro(int learn_int, int pause_int)
{

  pid_t pid, wait_pid;
  int status;
  int interval = 6;

  pid = fork();

  // if child
  if (pid == 0)
  {
      printf("This functions is still WIP!\n");
  }
  return;
}
