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
#define QUANTUM 100    /* number of milliseconds */

typedef struct process Process;
int USR1_received = 0;
int ALRM_received = 0;
Process *curr_running;

/* process struct holds pid and acts as node to doubly LList (Programs) */
typedef struct process {
    int first;
    pid_t pid;
    Process *next;
    Process *prev;
} Process;

/* Programs struct acts as doubly linked list; nodes are processes */
typedef struct programs {
    int num_of_programs;
    Process *head;
    Process *tail;
} Programs;


void signal_handler(int sig){
    switch (sig){
        case SIGALRM:
            //p1putstr(1, "Timer fired.\n");
            printf("Timer fired. Stopping %d\n", curr_running->pid);
            kill(curr_running->pid, SIGSTOP);
            ALRM_received++;
            break;
        case SIGUSR1:
            USR1_received++;
            break;
    }
}


/* removes a newline character from a word */
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
    int num_of_words;
    int result;
    int word_location;
    char word[WORD_SIZE];
    char buf[BUF_SIZE];
    char *arg[MAX_ARGS];
    Process *curr;
    struct itimerval it_val;

    /* attaching handlers to signal calls */
    if (signal(SIGALRM, signal_handler) == SIG_ERR) {
        p1perror(1, "Unable to catch SIGALARM");
        _exit(1);
    }
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        p1perror(1, "Unable to catch SIGUSR1");
        exit(1);
    }

    /* initializing timer */
    it_val.it_value.tv_sec = QUANTUM / 1000;
    it_val.it_value.tv_usec = (1000 * QUANTUM) % 1000000;
    it_val.it_interval = it_val.it_value;

    /* creating program list to hold processes */
    Programs *list_of_programs = malloc(sizeof(Programs));
    list_of_programs->head = NULL;
    list_of_programs->tail = NULL;
    list_of_programs->num_of_programs = 0;

    /* reading in line by line until we hit EOF */
    while ((result = p1getline(0, buf, BUF_SIZE)) != 0) {
        word_location = 0;
        num_of_words = 0;
        word_location = p1getword(buf, word_location, word);
        arg[0] = p1strdup(word);
        num_of_words++;

        /* reading in words */
        while ((word_location = p1getword(buf, word_location, word)) != -1) {
            arg[num_of_words] = p1strdup(word);
            num_of_words++;
        }

        /* here we are removing the \n char from the last string*/
        char *c = arg[num_of_words - 1];
        remove_newline_char(c);
        arg[num_of_words] = NULL;

        /* creating process struct (node of LList) */
        Process *p;
        if ((p = (Process *) malloc(sizeof(Process))) == NULL) return -1;
        p->pid = fork();
        p->first = 1;
        p->next = NULL;
        p->prev = NULL;

        /* if list of programs has no programs, make it the first & last */
        if (list_of_programs->num_of_programs == 0) {
            list_of_programs->head = p;
            list_of_programs->tail = p;
            list_of_programs->num_of_programs++;
        } else {
            /* otherwise, add it to the end of the list of programs */
            list_of_programs->tail->next = p;
            p->prev = list_of_programs->tail;
            list_of_programs->tail = p;
            list_of_programs->tail->next = NULL;
            list_of_programs->num_of_programs++;
        }

        /* continue if fork was successful */
        if (list_of_programs->tail->pid == 0) {
            while (!USR1_received)
                sleep(1);
            if (execvp(arg[0], arg) < 0) {
                printf("failed\n");
                for (i = 0; i < num_of_words; i++) {
                    free(arg[i]);
                }

                /* freeing nodes (processes) and the list struct as execvp failed */
                curr = list_of_programs->head;
                while (curr != NULL) {
                    if (curr->next != NULL) {
                        Process *temp = curr->next;
                        free(curr);
                        curr = temp;
                    } else {
                        free(curr);
                        curr = NULL;
                    }
                }
                free(list_of_programs);
                exit(1);
            }
        }else if(list_of_programs->tail->pid < 0){
            /* fork failed */
            p1perror(1, "Fork Failed");
        }

        /* freeing strings stored with strdup */
        for (i = 0; i < num_of_words; i++) {
            free(arg[i]);
        }
    }

    // printf("number of programs: %d\n", list_of_programs->num_of_programs);

    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        p1perror(1, "Error calling setitimer()");
        _exit(1);
    }

    /* Round Robin execution
     * we continue going through the programs until they have all executed */
    curr = list_of_programs->head;
    while (list_of_programs->num_of_programs > 0) {
        /* if equal to null, we are at the end, go back to front */
        if (curr == NULL) {
            curr = list_of_programs->head;
        }

        /* set the running pid to global */
        curr_running = curr;
        if (curr_running->first) {
            kill(curr_running->pid, SIGUSR1);
            curr_running->first = 0;
        } else {
            kill(curr_running->pid, SIGCONT);
        }

        /* do nothing while we wait for alarm */
        while (!ALRM_received);

        /* once alarm is set off, see if child is done */
        if (waitpid(curr_running->pid, NULL, WNOHANG) != 0) {
            printf("child finished\n");
            /* if child is done, we need to remove it, so here we are seeing its position
             * before we remove it from linked list */
            if (curr_running == list_of_programs->head) {
                /* we are done with process at the head */
                list_of_programs->head = curr_running->next;
                if (list_of_programs->head != NULL)
                    list_of_programs->head->prev = NULL;
            } else if (curr_running == list_of_programs->tail) {
                /* we are done with process at tail */
                list_of_programs->tail = curr_running->prev;
                if (list_of_programs->tail != NULL)
                    list_of_programs->tail->next = NULL;
            } else {
                /* process is somewhere in the middle */
                curr_running->prev->next = curr_running->next;
                curr_running->next->prev = curr_running->prev;
            }
            list_of_programs->num_of_programs--;

            /* freeing finished node */
            Process *temp = curr;
            curr = curr->next;
            free(temp);
        } else if (list_of_programs->num_of_programs != 0) {
            curr = curr->next;
        }

        /* reset alarm for next */
        ALRM_received = 0;
    }

    /* freeing struct */
    free(list_of_programs);

    return 1;
}