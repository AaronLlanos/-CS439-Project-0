#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "util.h"


/*
 * First, print out the process ID of this process.
 *
 * Then, set up the signal handler so that ^C causes
 * the program to print "Nice try.\n" and continue looping.
 *
 * Finally, loop forever, printing "Still here\n" once every
 * second.
 */
void handler(int sig);
/*
 * First, print out the process ID of this process.
 *
 * Then, set up the signal handler so that ^C causes
 * the program to print "Nice try.\n" and continue looping.
 *
 * Finally, loop forever, printing "Still here\n" once every
 * second.
 */
int main(int argc, char **argv)
{

struct timespec timer;

timer.tv_sec=0;
timer.tv_nsec=1000000000;


	pid_t pid;
	pid = getpid();
	printf("%i\n", pid);
	Signal(SIGALRM, handler); /* Install SIGALRM handler */
	Signal(SIGINT, handler); /* Install SIGINT handler */
	Signal(SIGUSR1, handler); /* Install SIGINT handler */

	alarm(1); /* Next SIGALRM will be delivered in 1s */

while (1) {
	nanosleep(&timer,NULL);
; /* Signal handler returns control here each time */
}
exit(0);


  return 0;
}


void handler(int sig)
{
	if(sig == SIGALRM)
	{
		ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "Still here.\n", 13); 
	if(bytes != 13) 
   	exit(-999);
   alarm(1);
	}
	else if(sig == SIGINT)
	{

			ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "nice try.\n", 12); 
	if(bytes != 10) 
   	exit(-999);
    alarm(1);
	}
	else if(sig == SIGUSR1)
	{
			ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "exiting\n", 12); 
	if(bytes != 10) 
   	exit(-999);
   alarm(1);
	}
	
}


