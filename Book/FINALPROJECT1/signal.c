#include <sys/types.h>
#include <unistd.h>
#include "p1fxns.h"
#include <signal.h>

#define UNUSED __attribute__ ((unused))

void sig_handler(UNUSED int signo) {
}

int main(UNUSED int argc, UNUSED char *argv[]) {
    sigset_t theSet;
    siginfo_t theInfo;
    pid_t thePid;

    thePid = getpid();
    p1putint(1, thePid);
    p1putstr(1, ": my pid\n");
    if (signal(SIGUSR1, sig_handler)== SIG_ERR)
        p1putstr(1, "can't catch SIGUSR1\n");
    sigemptyset(&theSet);
    sigaddset(&theSet, SIGUSR1);
    p1putstr(1, "Waiting for SIGUSR1\n");
    sigwaitinfo(&theSet, &theInfo);
    p1putstr(1, "Received SIGUSR1\n");
    return 0;
}
