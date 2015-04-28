//
// Created by benjamin barnes on 4/27/15.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "p1fxns.h"

#define BUF_SIZE 1024
#define WORD_SIZE 512
#define MAX_ARGS 512
#define MAX_NUM_PID 1024

//int main(int argc, const char *args[]) {
int main(){
    int i;
    int j;
    int k = 0;
    int result;
    int num_of_prog;
    int word_location;
    char word[WORD_SIZE];
    char buf[BUF_SIZE];
    //char *arg[MAX_ARGS][WORD_SIZE];
    char *arg[MAX_ARGS];
    //char *program[MAX_ARGS][WORD_SIZE];
    char *ptr;
    pid_t pid[MAX_NUM_PID];

    num_of_prog = 0;

    while ((result = p1getline(0, buf, BUF_SIZE)) != 0) {
        word_location = 0;
        j = 0;
        word_location = p1getword(buf, word_location, word);
        arg[0] = p1strdup(word);
        ptr = &word[0];
        for (i = 0; i < p1strlen(ptr); i++) word[i] = '0';
        p1putstr(1, arg[0]);
        printf("\n");
        j++;
        while ((word_location = p1getword(buf, word_location, word)) != -1) {
            arg[j] = p1strdup(word);
            ptr = &word[0];
            for (i = 0; i < p1strlen(ptr); i++) word[i] = '0';
            p1putstr(1, arg[j]);
            j++;
        }
        arg[j] = NULL;

        pid[num_of_prog] = fork();
        if(pid[num_of_prog] == 0){
            printf("Calling execute\n");
            execvp(arg[0], arg);
        }
        for(i = 0; i < j; i++){
            free(arg[i]);
        }
        num_of_prog++;
    }

    for(i = 0; i < num_of_prog; i++){
        //wait(pid[i]);
    }

/*
    num_of_prog = 0;


    while((result = p1getline(0, buf, BUF_SIZE)) != 0) {
        word_location = 0;
        j = 0;
//        word_location = p1getword(buf, word_location, program[num_of_prog]);
        while ((word_location = p1getword(buf, word_location, word)) != -1) {
            arg[num_of_prog][j] = word;
            p1putstr(1, arg[num_of_prog][j]);
            ptr = &word[0];
            for (i = 0; i < p1strlen(ptr); i++) word[i] = '0';
            //word_location = p1getword(buf, word_location, arg[num_of_prog]);
            //p1putstr(1,arg[num_of_prog][j]);
            //printf("word^%d\n",word_location);

            printf("\n");
            j++;
        }
        arg[num_of_prog][j] = (char *) NULL;

        pid[i] = fork();

        if (pid[i] == 0) {
            printf("Calling execute\n");
//            k = execvp(arg[num_of_prog][0], arg[num_of_prog]);
            k = execvp((const char *)(arg[num_of_prog][0]), arg[num_of_prog]);
            printf("%d\n",k);
            //execvp(*(program[num_of_prog]), (char * const*)*(arg[num_of_prog]));

            num_of_prog++;
            result = p1getline(0, buf, BUF_SIZE);
        }
    }
*/

    //printf("%d\n", num_of_prog);
/*
    for(i = 0; i < num_of_prog; i++){
//        j = 0;
        pid[i] = fork();
        //printf("%d\n",pid[i]);
        if(pid[i] == 0){
//            char* pt = &program;
//            printf("child\n");
//            while(word_location != -1){
//                word_location = p1getword(buf, word_location, arg[i][j]);
//                j++;
//            }
            //argss[j] = NULL;
            printf("Calling execute\n");
            //printf("")
            execvp(arg[num_of_prog][0], arg[num_of_prog]);
            //execvp(*(program[num_of_prog]), (char * const*)*(arg[num_of_prog]));
        }

    }

    for(i = 0; i <num_of_prog; i++){
        //wait(pid[i]);
    }
    */
    return 1;
}