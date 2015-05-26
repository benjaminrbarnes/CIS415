#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	wait(20);
	printf("I am your: \n");
	printf("	My PID %d.\n", getpid());

	if(argc < 2)
	{
		printf("Error: What am I? \nPass in at least one more string.");
		return 1;
	}
	int i;

	for(i=1; i < argc; i++)
	{
	   printf("%s ", argv[i]);
	}
	printf("\n");

	return 0;
}
