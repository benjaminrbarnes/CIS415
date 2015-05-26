#include <stdio.h>
#include <unistd.h>

int main()
{
	wait(15);
	printf("Hello World...\n");

	printf("	My PID %d.\n", getpid());

	return 0;

}
