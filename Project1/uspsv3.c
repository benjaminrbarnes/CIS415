//
// Created by benjamin barnes on 4/28/15.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "p1fxns.h"

#define BUF_SIZE 1024
#define WORD_SIZE 512
#define MAX_ARGS 512
#define MAX_NUM_PID 1024


typedef struct process{
    int done;
    pid_t pid;
    //Process* next;
}Process;



int USR1_received = 0;
Process *curr_running;



void USR1_handler(int signo) {
    USR1_received++;
}

void ALRM_handler(int signo){
    /*
    int status;
    pid_t result;
    result = waitpid(curr_running->pid, &status, WNOHANG) == 0
    if(result == 0){
        // child is alive, pause it
        kill(curr_running->pid, SIGSTOP);
    }else if(result == -1){
        // child is finished
        curr_running->done = 1;
    }
     */
    kill(curr_running->pid, SIGSTOP);
    printf("alarming\n");
}

void remove_newline_char(char *char_ptr){
    while(1){
        if(*char_ptr == '\0'){
            char_ptr--;
            if(*char_ptr == '\n'){
                *char_ptr = '\0';
            }
            break;
        }
        char_ptr++;
    }
}

//int main(int argc, const char *args[]) {
int main(){
    int i;
    int j;
    int result;
    int num_of_prog;
    int word_location;
    char word[WORD_SIZE];
    char buf[BUF_SIZE];
    char *arg[MAX_ARGS];
    //pid_t pid[MAX_NUM_PID];
    Process *pid[MAX_NUM_PID];
    num_of_prog = 0;

    if (signal(SIGUSR1, USR1_handler) == SIG_ERR
            || signal(SIGALRM, ALRM_handler) == SIG_ERR) {
        /* inform the caller and kill your program */
        return -1;
    }

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
        char* c = arg[j-1];
        remove_newline_char(c);
        arg[j] = NULL;
        Process *p;
        p = malloc(sizeof(Process));
        //Process *p = (* Process)malloc(sizeof(Process));

        //pid[num_of_prog] = fork();
        p->pid = fork();
        p->done = 0;
        pid[num_of_prog] = p;
        pid[num_of_prog + 1] = NULL;
        if(pid[num_of_prog]->pid == 0){
            while (! USR1_received)
                sleep(1);
            execvp(arg[0], arg);
            printf("done\n");
        }

        /* freeing strings stored with malloc */
        for(i = 0; i < j; i++){
            free(arg[i]);
        }
        num_of_prog++;
    }

    for(i = 0; i < num_of_prog; i++){
        kill(pid[i]->pid, SIGUSR1);
    }
    for(i = 0; i < num_of_prog; i++){
        kill(pid[i]->pid, SIGSTOP);
    }
    //sleep(3);

    int amt_done = 0;
    i = 0;
    while(amt_done != num_of_prog){
        if(i == num_of_prog) i = 0;
        // execute thing
        if(pid[i]->done == 0) {
            kill(pid[i]->pid, SIGCONT);
            curr_running = pid[i];
            // alarm stops current running process
            alarm(2);
            int status;
            if(waitpid(curr_running->pid, &status, WNOHANG) == 0){
                //printf("child is alive\n");
                // child is alive,
            }else{
                //printf("child is done\n");
                // it is done?
                curr_running->done = 1;
                amt_done++;
            }
            //if(curr_running->done == 1) amt_done++;
            //set alarm
            // see if done
            // if so increment done
            // change its process attribute
        }
        i++;


    }

    //for(i = 0; i < num_of_prog; i++){
//        kill(pid[i]->pid, SIGCONT);
//    }
    for(i = 0; i < num_of_prog; i++){
        int status;
        waitpid(pid[i]->pid, &status, 0);
        free(pid[i]);
    }
    return 1;
}