/*
 * Bogosort Array Shuffler
 * This program is totally useless!
 */

#include <stdio.h>
#include <stdlib.h>

void shuffle(int *arr, int arrLength)
{
        int i, swapInt, randInt;
        for(i = 0; i < arrLength; i++)
        {
                swapInt = arr[i];
                randInt = rand() % arrLength;
                arr[i] = arr[randInt];
                arr[randInt] = swapInt;
        }
}

int isSorted(int *arr, int arrLength)
{
        /* 1 = true; -1 = false */
        int sort;
        sort = 1;
        int i;

        for (i = 0; i < arrLength - 1; i++)
        {
                printf("%d ", arr[i]);
                if (arr[i + 1] < arr[i])
                        sort = -1;
        }
        printf("%d\n", arr[arrLength - 1]);
        return sort;
}

int main(int argc, char *argv[])
{
        int arr[argc];
	int a;
	for(a=1; a < argc; a++)
	{
		arr[a-1] = atoi(argv[a]);
		printf("%d\n", arr[a-1]);
	}
	
        int arrLength;
        arrLength = sizeof(arr) / sizeof(arr[0]);

        while (isSorted(arr, arrLength) == -1)
        {
                shuffle(arr, arrLength);
        }

        int i;
        for (i = 0; i < arrLength; i++)
        {
                printf("%d ", arr[i]);
        }
        printf("\n");
}
