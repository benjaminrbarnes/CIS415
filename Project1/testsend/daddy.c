#include <stdio.h>
#include <unistd.h>

int main()
{
	wait(5);
	printf("Hello Daddy!\n");

	printf("	My PID %d.\n", getpid());

	return 0;

}
