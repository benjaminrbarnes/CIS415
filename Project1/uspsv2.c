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

int USR1_received = 0;

void USR1_handler(int signo) {
    USR1_received++;
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
    pid_t pid[MAX_NUM_PID];
    num_of_prog = 0;

    if (signal(SIGUSR1, USR1_handler) == SIG_ERR) {
        /* inform the caller and kill your program */
        return -1;
    }

//    sigset_t signal_set;
//    sigemptyset (&signal_set);
//    sigaddset (&signal_set, SIGUSR1);

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
        pid[num_of_prog] = fork();
        if(pid[num_of_prog] == 0){
            while (! USR1_received)
                sleep(1);
            //execvp(file, args);
            /*
            int* k = NULL;
            int f = 0;
            printf("forked correctly \n");
            if((f = sigwait(&signal_set, k)) == 0) printf("successful wait\n");  // sig wait
            printf("should be 0 if sigwait was successful i think? : %d\n",f);
             */
            //printf("num\n");
            execvp(arg[0], arg);
        }
        /* freeing strings stored with malloc */
        for(i = 0; i < j; i++){
            free(arg[i]);
        }
        num_of_prog++;
    }

    for(i = 0; i < num_of_prog; i++){
        //int status;
        //signal(SIGUSR1, signal_handler);
        kill(pid[i], SIGUSR1);
        //printf("should be 0 if worked.. : %d \n",status);
        //waitpid(pid[i], &status, 0);
    }
    for(i = 0; i < num_of_prog; i++){
        kill(pid[i], SIGSTOP);
    }
    for(i = 0; i < num_of_prog; i++){
        kill(pid[i], SIGCONT);
    }
    for(i = 0; i < num_of_prog; i++){
        int status;
        waitpid(pid[i], &status, 0);
    }
    return 1;
}