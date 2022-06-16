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
  printf(GRN "\n     ==============================================================\n" CYN);
  printf("     /$$   /$$           /$$           /$$                 /$$ /$$\n\
    | $$  | $$          |__/          | $$                | $$| $$\n\
    | $$  | $$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$\n\
    | $$  | $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$\n\
    | $$  | $$| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$\n\
    | $$  | $$| $$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$| $$\n\
    |  $$$$$$/| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$\n\
     \\______/ |__/  |__/|__/|_______/ |__/  |__/ \\_______/|__/|__/\n" GRN);
  printf("     ==============================================================\n");
  printf(GRN "\n========================================================================\n");
  printf(CYN "                 The Unishell for students from students                 " GRN "\n");
  printf(CYN "  Our improved shell will make you feel like learning could never stop!  " GRN "\n");
  printf(CYN "                     Happy learning" RED " %s ( ˘ ³˘)♥" GRN "                             " GRN "\n", getusername());
  printf("========================================================================\n \n");

  return 1;
}

/**
 * @brief start unimode by changing the /etc/hosts file
 * to /etc/hosts.old if activated. This is to prevent the
 * user from accessing specific websites.
 *
 * When the user activates the shell again, the /etc/hosts.old
 * gets changed to /etc/hosts and the user can access the websites.
 *
 * @param learning boolean to check if the user is learning or not
 * @return int
 */
int unimode(int learning)
{
   if (access("hosts", F_OK) != 0 && learning == 0) {
          learning = 1;
  }
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
    return 1;
  } // if unimode on
  else
  {

    // revert changes
    if (rename("/etc/hosts", "hosts") != 0)
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

  return 0;
}

/**
 * @brief Prints the command line with time left from the pomodoro timer
 *
 * This basically is the same as in the main loop. It gets all the needed
 * data to print it. In addition it here prints with the time left from
 * the pomodoro timer. It flushed the stdout to make sure the output is
 * printed correctly. Because the OS only updates the terminal after a
 * line break is detected. Flushing the stream ensures that the os is
 * forced to update it.
 *
 * @param h hours left
 * @param m minutes left
 * @param s seconds left
 * @param pause is in pause mode
 * @param i current iteration
 * @param interval amount of intervals in total
 * @param line current line with input from the user to be added to command line
 * @param ip interface used to get the ip address
 */
void printWithTime(int h, int m, int s, int pause, int i, int interval, char *line, char *ip)
{
  char cwd[PATH_MAX];
  char hostname[HOST_NAME_MAX];

  gethostname(hostname, HOST_NAME_MAX);

  int hostname_len = strlen(hostname);
  int username_len = strlen(getusername());
  getcwd(cwd, sizeof(cwd));

  printf("\033[A\r" GRN "(" YEL "%s" CYN "@" RED "%s" GRN ")┬" GRN "[" CYN "%s" GRN "]―" GRN "[" CYN "%s" GRN "] {%s%02d:%02d:%02d - %s (%d/%d)" GRN "} \n%*c└─" CYN "₿ %s",
         hostname,
         getusername(),
         get_ip(ip),
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

/**
 * @brief Struct to give data to the thread
 */
struct pomodoro_args
{
  int learn_int;
  int pause_int;
  char *line;
  char *ip;
};

/**
 * @brief Pomodoro thread keeps track of past time and updates command line
 *
 * Has all loops needed for pomodoro to work. Updates line with left time and
 * then sleeps for one second (So actually not real clock).
 *
 * @param _args to be used for pomodoro timer
 */
void *pomodoroThread(void *_args)
{
  sleep(0.5);
  int status;
  int interval = 6;
  struct pomodoro_args *args = (struct pomodoro_args *)_args;
  int learn_int = args->learn_int * 60000;
  int pause_int = args->pause_int * 60000;
  char *line = args->line;
  char *ip = args->ip;
  free(args);

  for (int i = 0; i < interval; i++)
  {
    // Study
    int time = 0;
    do
    {
      int left = learn_int - time;
      printWithTime(left / 3600000, left / 60000 % 60, left / 1000 % 60, 0, i + 1, interval, line, ip);
      sleep(1);
      time += 1000;
    } while (time <= learn_int);

    // Pause
    time = 0;
    do
    {
      int left = pause_int - time;
      printWithTime(left / 3600000, left / 60000 % 60, left / 1000 % 60, 1, i + 1, interval, line, ip);
      sleep(1);
      time += 1000;
    } while (time <= pause_int);
  }

  return NULL;
}

/**
 * @brief Main pomodoro function that starts pomodoro thread
 *
 * @param _learn_int learn duration per intervall
 * @param _pause_int pause duration per intervall
 * @param line pointer to line being buffered
 * @param ip pointer to interface
 */
void pomodoro(int _learn_int, int _pause_int, char *line, char *ip)
{
  printf("\nStarting Pomodoro with %dmin learn and %dmin pause interval ☜(⌒▽⌒)☞\n\n", _learn_int, _pause_int);

  pthread_t thread_id;
  struct pomodoro_args *args = malloc(sizeof(struct pomodoro_args));
  args->learn_int = _learn_int;
  args->pause_int = _pause_int;
  args->line = line;
  args->ip = ip;
  pthread_create(&thread_id, NULL, pomodoroThread, args);

  return;
}

/**
 * @brief gets the IP address of the computer
 *
 * This function gets the IP address of the computer with ioctl(). It then parses the
 * result and returns the IP address.
 *
 * from stackoverflow: https://stackoverflow.com/questions/2283494/get-ip-address-of-an-interface-on-linux
 *
 * @param ip pointer to interface
 * @return char* the IP address
 *

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

/**
 * @brief help function that prints the help message
 *
 * @param args not relevant for this command
 * @return int
 */
int us_help(char **args)
{
  printf(GRN "\n                |======================================|\n");
  printf("                |" CYN " The following commands are built in: " GRN "|\n");
  printf(GRN "                |======================================|\n" YEL);

  // get lenght of string

  for (int i = 0; i < size_intern_strings(); i++)
  {
    printf(GRN "                |" YEL);
    printf("             %d. %s", i + 1, intern_strings[i]);
    printf(GRN "            %*c|\n", 10 - strlen(intern_strings[i]), ' ');
  }
  printf(GRN "                |======================================|\n\n");

  return 1;
}