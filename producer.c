// producer.c
// Executable that will attempt to fill buffers in shared memory by reading text file
// Alex Kane
// 2/10/2018
// CS 4760 Project 2
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "function_library.h"

int main(int argc, char** argv)
{
	int* turn = getExistingSharedMemory(1, argv[0]);
	int* numProcesses = getExistingSharedMemory(2, argv[0]);
	int* flags = getExistingSharedMemory(3, argv[0]);
	*turn = 1;
	*numProcesses = 1;
	*flags = 1;
	printf("turn: %d\n", *turn);
	printf("numProcesses: %d\n", *numProcesses);
	printf("flags: %d\n", *flags);
}
