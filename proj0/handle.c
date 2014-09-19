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


void alarm_handler(int sig);
void int_handler(int sig);
void usr1_handler(int sig);

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
	Signal(SIGALRM, alarm_handler); /* Install SIGALRM handler */
	Signal(SIGINT, int_handler);  /* Install SIGINT handler */
	Signal(SIGUSR1, usr1_handler); /* Install SIGUSR1 handler */

	alarm(1); /* Next SIGALRM will be delivered in 1s */

	while (1) {
		nanosleep(&timer,NULL);  /* Signal handler returns control here each time */
		continue;
	}
	exit(0);
}


void alarm_handler(int sig)
{
	ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "Still here.\n", 12); 
	if(bytes != 12) 
   		exit(-999);
   	alarm(1);
	
}
void int_handler(int sig)
{

	ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "Nice try.\n", 10); 
	if(bytes != 10) 
   		exit(-999);
    alarm(1);
}
void usr1_handler(int sig)
{
	ssize_t bytes; 
	const int STDOUT = 1; 
	bytes = write(STDOUT, "exiting\n", 8); 
	if(bytes != 8) 
   		exit(1);
   	exit(1);
}


