//
// Created by benjamin barnes on 4/27/15.
//
#include <stdio.h>
int main(){
    int i = 0;
    while(i < 2){
        printf("%d on iteration: %d\n",getpid(),i);
        i++;
        sleep(3);
    }

    return 1;
}