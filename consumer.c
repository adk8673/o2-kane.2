// consumer.c
// Executable which will read from buffers filled by producer
// Alex Kane
// 2/10/2018
// CS 4760 Project2
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>

#include"function_library.h"

enum state { idle = 0, want_in = 1, in_cs = 2};
void process(const int, const int, int*, int*);

int main(int argc, char** argv)
{
	int* turn = getExistingSharedMemory(1, argv[0]);
	int* numProcesses = getExistingSharedMemory(3, argv[0]);
	int* flags = getExistingSharedMemory(2, argv[0]);
	int* bufferFlag1 = getExistingSharedMemory(4, argv[0]);
	char* buffer1 = getExistingSharedMemory(5, argv[0]);
	int* bufferFlag2 = getExistingSharedMemory(6, argv[0]);
	char* buffer2 = getExistingSharedMemory(7, argv[0]);
	int* bufferFlag3 = getExistingSharedMemory(8, argv[0]);
	char* buffer3 = getExistingSharedMemory(9, argv[0]);
	int* bufferFlag4 = getExistingSharedMemory(10, argv[0]);
	char* buffer4 = getExistingSharedMemory(11, argv[0]);
	int* bufferFlag5 = getExistingSharedMemory(12, argv[0]);
	char* buffer5 = getExistingSharedMemory(13, argv[0]);

	int turnNumber = 0;
	if (argc > 0)
		turnNumber = atoi(argv[1]);
	
	process(turnNumber, *numProcesses, turn, flags);

	return 0;
}
/*
void process(const int i, const int n, int* turn, int* flag)
{
	int j;
	do 
	{
		do
		{
			flag[i] = (int)want_in;
			j = *turn;
			while (j != i)
				j = (flag[j] != idle) ? *turn : (j + 1) % n;
			printf("test2\n");
			// declare intention to enter critical section
			flag[i] = in_cs;

			// check that no one else is in the critical section
			for (j = 0; j < n; j++)
				if ((j != i) && (flag[j] == in_cs))
					break;
			printf("Turn in consumer: %d\n", *turn);
		} while ((j < n) || (*turn != i && flag[*turn] != idle));
	
		printf("test 1 consumer\n");	
		// Assign turn to self and enter critical section
		*turn = i;
		printf("Consumer %d in critical section\n", i);
		sleep(3);

		//Exit section
		j = (*turn + 1) % n;
		printf("after cirtical\n");
		while (flag[j] == idle)
			j = (j + 1) % n;
		
		//Assign turn to next waiting process; change own flag to idle 
		*turn = j; flag[i] = idle;
		sleep(2);
	}while(1); 
}*/
