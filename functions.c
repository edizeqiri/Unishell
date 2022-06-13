#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "functions.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

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

int us_help(char **args)
{

  int i;
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
    printf("To leave The ZEN ᕙ(⇀‸↼)ᕗ just type the command 'unimode' again.\n");
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
    printf("To start The ZEN ᕙ(⇀‸↼)ᕗ again just type the command 'unimode'.\n");
  }

  return 1;
}

void printWithTime(int h, int m, int s, int pause, int i, int interval, char* line)
{
  char cwd[PATH_MAX];
  char hostname[HOST_NAME_MAX];

  gethostname(hostname, HOST_NAME_MAX);

  int hostname_len = strlen(hostname);
  int username_len = strlen(getusername());
  getcwd(cwd, sizeof(cwd));

  printf("\033[A\r" GRN "(" YEL "%s" CYN "@" RED "%s" GRN ")┬" GRN "[" CYN "%s" GRN "] {%s%02d:%02d:%02d - %s (%d/%d)" GRN "} \n%*c└─" CYN "₿ %s", 
    hostname,
    getusername(),
    cwd,
    pause ? MAG : BLU,
    h, m, s,
    pause ? "Rest" : "Study",
    i, interval,
    (hostname_len + username_len + 3),
    ' ',
    line);

  fflush(stdout);
}

struct pomodoro_args
{
  int learn_int;
  int pause_int;
  char* line;
};

void *pomodoroThread(void *_args)
{
  sleep(0.5);
  int status;
  int interval = 6;
  struct pomodoro_args *args = (struct pomodoro_args *) _args;
  int learn_int = args->learn_int * 60000;
  int pause_int = args->pause_int * 60000;
  char *line = args->line;
  free(args);

  for (int i = 0; i < interval; i++)
  {
    // Study
    int time = 0;
    do {
      int left = learn_int - time;
      printWithTime(left/3600000, left/60000 % 60, left/1000 % 60, 0, i+1, interval, line);
      sleep(1);
      time += 1000;
    } while (time <= learn_int);

    // Pause
    time = 0;
    do {
      int left = pause_int - time;
      printWithTime(left/3600000, left/60000 % 60, left/1000 % 60, 1, i+1, interval, line);
      sleep(1);
      time += 1000;
    } while (time <= pause_int);
  }
  
  return NULL;
}

void pomodoro(int _learn_int, int _pause_int, char* line)
{
  printf("\nStarting Pomodoro with %dmin learn and %dmin pause interval ☜(⌒▽⌒)☞\n\n", _learn_int, _pause_int);

  pthread_t thread_id;
  struct pomodoro_args *args = malloc (sizeof (struct pomodoro_args));
  args->learn_int = _learn_int;
  args->pause_int = _pause_int;
  args->line = line;
  pthread_create(&thread_id, NULL, pomodoroThread, args);

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