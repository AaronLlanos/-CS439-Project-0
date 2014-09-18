#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char **argv)
{
  if(argc != 2){
  	exit(1);
  }
  else{
  	pid_t n = (pid_t)strtol(argv[1], NULL, 0);
  	int val = kill(n, SIGKILL);
  	if(val == -1){
  		printf("Couldnt find PID: %d\n", (int)n);
  		exit(1);
  	}
  	return 0;
  }
}
