#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

const int MAX = 13;

static void doFib(int n, int doPrint);


/*
 * unix_error - unix-style error routine.
 */
inline static void 
unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}


int main(int argc, char **argv)
{
  int arg;
  int print;

  if(argc != 2){
    fprintf(stderr, "Usage: fib <num>\n");
    exit(-1);
  }

  if(argc >= 3){
    print = 1;
  }

  arg = atoi(argv[1]);
  if(arg < 0 || arg > MAX){
    fprintf(stderr, "number must be between 0 and %d\n", MAX);
    exit(-1);
  }

  doFib(arg, 1);

  return 0;
}

/* 
 * Recursively compute the specified number. If print is
 * true, print it. Otherwise, provide it to my parent process.
 *
 * NOTE: The solution must be recursive and it must fork
 * a new child for each call. Each process should call
 * doFib() exactly once.
 */
static void doFib(int n, int doPrint)
{
  //Base case, return n if n<2
  if (n < 2){
    if(doPrint){
      printf("%d\n", n);
    }
    exit(n);
  }
  else{
    pid_t pid1, pid2;
    int status1, status2;

    pid1 = fork();

      /* Code executed by first child */
      if (pid1 == 0) {     
        doFib(n-1, 0);
      } 
      /* Code executed by first parent */
      else {     

        wait(&status1);
        //Reap first Child
        status1 = WEXITSTATUS(status1);

        pid2 = fork();
        /* Code executed by second child */
        if (pid2 == 0) {     
          doFib(n-2, 0);
        } 
        /* Code executed by second parent */
        else {       
          wait(&status2);
          //Reap second child
          status2 = WEXITSTATUS(status2);
          if(doPrint){
            printf("%d\n", status1+status2);
          }
          exit(status1+status2);
        }
      }
  }

}