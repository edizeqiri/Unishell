#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "functions.h"

/**
 * @brief change directory
 *
 * @param args
 * @return int
 */
int us_cd(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "Unishell: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
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
char *getusername()
{
  char *name;
  struct passwd *pass;
  pass = getpwuid(getuid());
  name = pass->pw_name;
  return name;
}

int welcome(char **args)
{
  printf(GRN "\n==============================================================\n" CYN);
  printf(" /$$   /$$           /$$           /$$                 /$$ /$$\n\
| $$  | $$          |__/          | $$                | $$| $$\n\
| $$  | $$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$\n\
| $$  | $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$\n\
| $$  | $$| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$\n\
| $$  | $$| $$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$| $$\n\
|  $$$$$$/| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$\n\
 \\______/ |__/  |__/|__/|_______/ |__/  |__/ \\_______/|__/|__/\n" GRN);
  printf("==============================================================\n");
  printf(GRN "\n|=========================================================================|\n");
  printf("|" CYN "                 The Unishell for students from students                 " GRN "|\n");
  printf("|" CYN "  Our improved shell will make you feel like learning could never stop!  " GRN "|\n");
  printf("|" CYN "                     Happy learning " RED "( ˘ ³˘)♥" GRN "                             " GRN "|\n", getusername());
  printf("|=========================================================================|\n \n");

  return 1;
}

int unimode(int learning)
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

/**
 * @brief gets the IP address of the computer
 * @return char* the IP address
 */
char *get_ip(char *interface)
{
  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  // get ipv4 address
  ifr.ifr_addr.sa_family = AF_INET;

  // attach to interface
  strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

  ioctl(fd, SIOCGIFADDR, &ifr);

  close(fd);

  return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

int size_intern_strings()
{
  return sizeof(intern_strings) / sizeof(char *);
}

int us_help(char **args)
{
  printf(GRN"\n                |======================================|\n");
  printf("                |"CYN " The following commands are built in: "GRN "|\n");
  printf(GRN"                |======================================|\n"YEL);

  // get lenght of string


  for (int i = 0; i < size_intern_strings(); i++)
  {
    printf(GRN"                |"YEL);
    printf("             %d. %s",i+1, intern_strings[i]);
    printf(GRN"            %*c|\n",10 - strlen(intern_strings[i]), ' ');
  }
   printf(GRN"                |======================================|\n\n");
}