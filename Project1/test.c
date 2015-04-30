//
// Created by benjamin barnes on 4/27/15.
//
#include <stdio.h>
int main(){
    int i = 0;
    while(i < 3){
        printf("%d on iteration: %d\n",getpid(),i);
        i++;
        sleep(2);
    }

    return 1;
}