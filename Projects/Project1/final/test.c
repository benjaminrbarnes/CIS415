//
// Created by benjamin barnes on 4/27/15.
//
#include <stdio.h>
int main(){
    int i = 0;
    while(i < 6){
        //printf("%d on iteration: %d\n",getpid(),i);
        i++;
        sleep(1);
    }

    return 1;
}