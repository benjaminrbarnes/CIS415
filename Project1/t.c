#include <stdio.h>
int test(int i){
	return i*i;
}

int main(){
	if(test(2) == 4){
		printf("hello\n");
	}

}