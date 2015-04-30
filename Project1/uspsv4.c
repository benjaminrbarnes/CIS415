//
// Created by benjamin barnes on 4/28/15.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "p1fxns.h"

#define BUF_SIZE 1024
#define WORD_SIZE 512
#define MAX_ARGS 512
#define MAX_NUM_PID 1024
#define QUANTUM 100	/* number of milliseconds */

typedef struct process Process;
int USR1_received = 0;
int ALRM_received = 0;
Process *curr_running;

typedef struct process {
    int first;
    pid_t pid;
    Process *next;
    Process *prev;
} Process;

typedef struct programs {
    int num_of_programs;
    Process *head;
    Process *tail;
} Programs;


void timer_handler( int sig) {
    p1putstr(1, "Timer fired.\n");
    kill(curr_running->pid, SIGSTOP);
    //printf("alarming\n");
    ALRM_received++;
}

void USR1_handler(int signo) {
    USR1_received++;
}


void ALRM_handler(int signo) {
    kill(curr_running->pid, SIGSTOP);
    printf("alarming\n");
    ALRM_received++;
}


/* child handler incase process finishes before alarm handler is called  */
void CHLD_handler(int signo){
    kill(curr_running->pid, SIGSTOP);
    //printf("Child\n");
    ALRM_received++;
}


void remove_newline_char(char *char_ptr) {
    while (1) {
        if (*char_ptr == '\0') {
            char_ptr--;
            if (*char_ptr == '\n') {
                *char_ptr = '\0';
            }
            break;
        }
        char_ptr++;
    }
}

//int main(int argc, const char *args[]) {
int main() {
    int i;
    int j;
    int result;
    int num_of_prog;
    int word_location;
    char word[WORD_SIZE];
    char buf[BUF_SIZE];
    char *arg[MAX_ARGS];
    Process *curr;
    num_of_prog = 0;

    struct itimerval it_val;

    if (signal(SIGALRM, timer_handler) == SIG_ERR) {
        p1perror(1, "Unable to catch SIGALARM");
        _exit(1);
    }

    it_val.it_value.tv_sec = QUANTUM / 1000;
    it_val.it_value.tv_usec = (1000 * QUANTUM) % 1000000;
    it_val.it_interval = it_val.it_value;


    if (signal(SIGUSR1, USR1_handler) == SIG_ERR){
        //|| signal(SIGALRM, ALRM_handler) == SIG_ERR){
//            || signal(SIGCHLD, ALRM_handler) == SIG_ERR) {
        /* inform the caller and kill your program */
        p1perror(1, "Unable to cath SIGUSR1");
        exit(1);
    }

    Programs *list_of_programs = malloc(sizeof(Programs));
    list_of_programs->head = NULL;
    list_of_programs->tail = NULL;
    list_of_programs->num_of_programs = 0;

    while ((result = p1getline(0, buf, BUF_SIZE)) != 0) {
        word_location = 0;
        j = 0;
        word_location = p1getword(buf, word_location, word);
        arg[0] = p1strdup(word);
        j++;
        while ((word_location = p1getword(buf, word_location, word)) != -1) {
            arg[j] = p1strdup(word);
            j++;
        }
        /* here we are removing the \n char from the last string*/
        char *c = arg[j - 1];
        remove_newline_char(c);
        arg[j] = NULL;


        Process *p;
        if((p = (Process *)malloc(sizeof(Process))) == NULL) return -1;
        p->pid = fork();
        p->first = 1;
        p->next = NULL;
        p->prev = NULL;

        if (list_of_programs->num_of_programs == 0) { // if list of programs has no programs, make it the first
            list_of_programs->head = p;
            list_of_programs->tail = p;
            list_of_programs->num_of_programs++;
        } else {        // otherwise, add it to the end of the list of programs
            list_of_programs->tail->next = p;
            p->prev = list_of_programs->tail;
            list_of_programs->tail = p;
            list_of_programs->tail->next = NULL;
            list_of_programs->num_of_programs++;
        }

        if (list_of_programs->tail->pid == 0) {
            while (!USR1_received)
                sleep(1);
            if(execvp(arg[0], arg) < 0){
                printf("failed\n");
//                for (i = 0; i < j; i++) {
//                    free(arg[i]);
//                }
                if(list_of_programs->head == p){
                    // if equal to head, we know it is only one b/c we add to tail
                    free(p);
                    free(list_of_programs);
                    list_of_programs = NULL;
                }else{
                    // we know it is the second or farther
                    list_of_programs->tail = list_of_programs->tail->prev;
                    list_of_programs->tail->next = NULL;
                    list_of_programs->num_of_programs--;
                    free(p);

                }
//                curr = list_of_programs->head;
//                while(curr != NULL){
//                    if(curr->next != NULL) {
//                        Process *temp = curr->next;
//                        free(curr);
//                        curr = temp;
//                    }else{
//                        free(curr);
//                        curr = NULL;
//                    }
//                }
//                free(list_of_programs);
            }
        }

        /* freeing strings stored with malloc */
        for (i = 0; i < j; i++) {
            free(arg[i]);
        }
        if(list_of_programs != NULL) {
            num_of_prog++;
        }
    }

    printf("number of programs: %d\n", num_of_prog);

    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        p1perror(1, "Error calling setitimer()");
        _exit(1);
    }
    if(list_of_programs != NULL) {
        curr = list_of_programs->head;
    }
    while (list_of_programs->num_of_programs > 0 && list_of_programs != NULL) {
        if (curr == NULL) {                       // if equal to null, we are at the end, go back to front
            curr = list_of_programs->head;
        }
        curr_running = curr;
        if(curr_running->first) {
            kill(curr_running->pid, SIGUSR1);
            curr_running->first = 0;
        }else {
            kill(curr_running->pid, SIGCONT);
        }
        //alarm(1);
        while (!ALRM_received) {
            // do nothing while we wait for alarm
        }
        if (waitpid(curr_running->pid, NULL, WNOHANG) != 0) { // child is done
            printf("child finished\n");
            if (curr_running == list_of_programs->head) { // we are done with process at the head
                list_of_programs->head = curr_running->next;
                if (list_of_programs->head != NULL)
                    list_of_programs->head->prev = NULL;
            } else if (curr_running == list_of_programs->tail) {   // we are done with process at tail
                list_of_programs->tail = curr_running->prev;
                if (list_of_programs->tail != NULL)
                    list_of_programs->tail->next = NULL;
            } else {                                              // process is somewhere in the middle
                curr_running->prev->next = curr_running->next;
                curr_running->next->prev = curr_running->prev;
            }
            list_of_programs->num_of_programs--;

            // freeing finished node
            Process *temp = curr;
            curr = curr->next;
            free(temp);
        }else if(list_of_programs->num_of_programs != 0){
                curr = curr->next;
        }
        ALRM_received = 0;
    }
    if(list_of_programs != NULL){
        printf("freeing programs\n");
        free(list_of_programs);
    }

    //return 1;
}