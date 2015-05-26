/* Simplest dead child cleanup in a SIGCHLD handler. Prevent zombie processes
 * but don't actually do anything with the information that a child died.
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "p1fxns.h"

#define NUM_OF_CHILDREN 10
#define UNUSED __attribute__ ((unused))
 
/* SIGCHLD handler. */
static void sigchld_handler (UNUSED int sig) {
	pid_t pid;
	int status;
	/* Wait for all dead processes.
	 * We use a non-blocking call to be sure this signal handler will not
	 * block if a child was cleaned up in another part of the program. */
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		if (WIFEXITED(status)) {
			p1putint(1, pid);
			p1putstr(1, ": exited\n");
		}
	}
}
 
int main (UNUSED int argc, UNUSED char *argv[])
{
	struct sigaction act;
	int i;
 
	memset (&act, 0, sizeof(act));
	act.sa_handler = sigchld_handler;
 
	if (sigaction(SIGCHLD, &act, 0)) {
		perror ("sigaction");
		return 1;
	}
 
	/* Make some children. */
	for (i = 0; i < NUM_OF_CHILDREN; i++) {
		switch (fork()) {
			case -1:
				perror ("fork");
				return 1;
			case 0:		/* child does nothing but exit */
				return 0;
		}
	}
 
	/* Wait until a sleep() call that is not interrupted by a signal. */
	while (sleep(1)) {
	}
 
	return 0;
}
