#include <stdio.h>
#include <unistd.h>

int main()
{
	wait(10);
	printf("Hello Mom!\n");

	printf("	My PID %d.\n", getpid());

	return 0;

}
