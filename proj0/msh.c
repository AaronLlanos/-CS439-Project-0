/* 
 * msh - A mini shell program with job control
 * 
 *  
    Program authors: Aaron Llanos, al26593(Log ID) 
                     Tarequl Alam, tarik90(Log ID) 
    Dates: 9/17, 9/18, 9/19
    Description of program: Shell program that runs basic 
    commands lines and four build-in command
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "util.h"
#include "jobs.h"


/* Global variables */
int verbose = 0;            /* if true, print additional output */

extern char **environ;      /* defined in libc */
static char prompt[] = "msh> ";    /* command line prompt (DO NOT CHANGE) */
static struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
void usage(void);
void sigquit_handler(int sig);



/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
            case 'h':             /* print help message */
                usage();
    	    break;
            case 'v':             /* emit additional diagnostic info */
                verbose = 1;
    	    break;
            case 'p':             /* don't print a prompt */
                emit_prompt = 0;  /* handy for automatic testing */
    	    break;
        	default:
                usage();
    	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

    	/* Read command line */
    	if (emit_prompt) {
    	    printf("%s", prompt);
    	    fflush(stdout);
    	}
    	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
    	    app_error("fgets error");
    	if (feof(stdin)) {/* End of file (ctrl-d) */
    	    fflush(stdout);
    	    exit(0);
    	}

    	/* Evaluate the command line */
    	eval(cmdline);
    	fflush(stdout);
    	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/

 //Aaron drove here
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];
    /* Holds modified command line */
    int bg;
    /* Should the job run in bg or fg? */
    pid_t pid;
    /* Process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    if (argv[0] == NULL)
        return;
        /* Ignore empty lines */

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    if (!builtin_cmd(argv)) {
        
        sigprocmask(SIG_BLOCK, &mask, NULL);

        if ((pid = fork()) == 0) {
        /* Child runs user job */
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            setpgid(0, 0);
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
        /* Parent waits for foreground job to terminate */
        if (!bg) {
            
            addjob(jobs, pid, 1, cmdline);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);

            waitfg(pid);

        }
        else{
            addjob(jobs, pid, 2, cmdline);
            printf("[%d] (%d) %s", pid2jid(jobs, pid), pid, cmdline);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
    }
    return;
//End of Aaron driving
}


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 * Return 1 if a builtin command was executed; return 0
 * if the argument passed in is *not* a builtin command.
 */

 //Tarequl driving now
int builtin_cmd(char **argv) 
{
    /*************************************
    ************************************** Build builtin_cmds: jobs, quit, bg <job>, fg <job>
    **************************************/

    /* quit command */   
    if (!strcmp(argv[0], "quit")) 
        exit(0);

    /* jobs command */   
    if (!strcmp(argv[0], "jobs")){
       listjobs(jobs);
       return 1;
    }

    /* bg<job> command */
    if (!strcmp(argv[0], "bg") && argv[1] != NULL){
        do_bgfg(argv);
        return 1;
    }
        

    /* fg<job> command */
    if (!strcmp(argv[0], "fg") && argv[1] != NULL){
        do_bgfg(argv);
        return 1;
    } 
        

    /*****************************************************************************************************/

    return 0;     /* not a builtin command */
}
//End of Tarequl driving, Aaron driving now
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    if(argv[1] == NULL){
        return;
    }
    //Run program in foreground
    if(!strcmp(argv[0], "fg"))
    {
        if (fgpid(jobs) !=0) 
        {
            kill(fgpid(jobs), SIGCONT);
        }

    }
    //Run program in background
    else if (!strcmp(argv[0], "bg")){

            kill(getpid(), SIGCONT);
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{

    while(fgpid(jobs) == pid){
        sleep(1);
    }
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    ssize_t bytes; 
    char str[50];
    const int STDOUT = 1; 

    while ((int)pid > 0){
        
        //if child process exited normally
        if(WIFEXITED(status)){
            deletejob(jobs, pid);
        }

        //if a child terminates by a signal raise by the child itself
        if(WIFSIGNALED(status)){
            sprintf(str, "Job [%d] (%d) terminated by signal %d\n", pid2jid(jobs, pid), pid, WTERMSIG(status));
            bytes = write(STDOUT, str, strlen(str)); 
            if(bytes != strlen(str)) 
                exit(-999);
            deletejob(jobs, pid);
        }

        //if the child process is currently stopped
        if (WIFSTOPPED(status)){
            //getting job id of a stopped job
            getjobpid(jobs, pid)->state = 3;
            sprintf(str, "Job [%d] (%d) stopped by signal %d\n", pid2jid(jobs, pid), pid, WSTOPSIG(status));
            bytes = write(STDOUT, str, strlen(str)); 
            if(bytes != strlen(str)) 
                exit(-999); 
            return; 
        }
        else{
            return;
        }
    }

    return;
}

//End of Arron driving
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */

//Aaron and Tarequl drove here
void sigint_handler(int sig) 
{
    pid_t fgpid2;
    
    //checking for forgeground job
    fgpid2 = fgpid(jobs);
    if(!fgpid2){
        return;
    }
    //passing signal to child
    kill(-fgpid2, SIGINT);
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */

 //SIGSTOP handler 
void sigtstp_handler(int sig) 
{
    pid_t fgpid2;
    
    //checking for forgeground job
    fgpid2 = fgpid(jobs);
    if(!fgpid2){
        return;
    }
    //passing signal to child
    kill(-fgpid2, SIGTSTP);

    return;
}

/*********************
 * End signal handlers
 *********************/



/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    ssize_t bytes;
    const int STDOUT = 1;
    bytes = write(STDOUT, "Terminating after receipt of SIGQUIT signal\n", 45);
    if(bytes != 45)
       exit(-999);
    exit(1);
}



