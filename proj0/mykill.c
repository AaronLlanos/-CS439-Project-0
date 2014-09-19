/*  Program authors: Aaron Llanos, al26593(Log ID) 
                     Tarequl Alam, tarik90(Log ID) 
    Dates: 9/17
    Description of program: Program to send SIGUSR1 signal 
    to specific PID
*/

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

//Aaron driving now
int main(int argc, char **argv)
{
  if(argc != 2){
  	exit(1);
  }
  else{
  	pid_t n = (pid_t)atoi(argv[1]);
  	int val = kill(n, SIGUSR1);
    // If pid not found
  	if(val == -1){
  		printf("Couldnt find PID: %d\n", (int)n);
  		exit(1);
  	}
  	return 0;
  }
}
//End of Aaron driving
