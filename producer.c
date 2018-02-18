// producer.c
// Executable that will attempt to fill buffers in shared memory by reading text file
// Alex Kane
// 2/10/2018
// CS 4760 Project 2
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "function_library.h"

int main(int argc, char** argv)
{
	key_t kTurn = getKey(1);
	printf("%d\n", kTurn);
	int shmidTurn = shmget(kTurn, 0, 0);
	printf("%d\n", shmidTurn);
	int* turn;
	turn = (int*)shmat(shmidTurn, NULL, 0);
	printf("%d\n", turn);
	printf("%d\n", errno);
	/**turn = 1;
	printf("%d\n", turn);*/	
	return 0;
}
