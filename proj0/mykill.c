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
  	pid_t n = (pid_t)atoi(argv[1]);
  	int val = kill(n, SIGUSR1);
  	if(val == -1){
  		printf("Couldnt find PID: %d\n", (int)n);
  		exit(1);
  	}
  	return 0;
  }
}
