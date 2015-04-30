#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "p1fxns.h"

#define QUANTUM 100	/* number of milliseconds */
#define UNUSED __attribute__ ((unused))

void timer_handler(UNUSED int sig) {
    p1putstr(1, "Timer fired.\n");

}

int main(UNUSED int argc, UNUSED char *argv[]) {

    struct itimerval it_val;

    if (signal(SIGALRM, timer_handler) == SIG_ERR) {
        p1perror(1, "Unable to catch SIGALARM");
        _exit(1);
    }
    it_val.it_value.tv_sec = QUANTUM / 1000;
    it_val.it_value.tv_usec = (1000 * QUANTUM) % 1000000;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        p1perror(1, "Error calling setitimer()");
        _exit(1);
    }

    while (1)
        pause();

}
