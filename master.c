// master.c
// Code for master executable which will be used to spawn producer and consumer children as well as handle
// signals
// Alex Kane
// 2/10/18
// CS 4760 Project 2
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<signal.h>
#include<time.h>
#include"buffer_size.h"
#include"function_library.h"

void displayHelp();
void deallocateAllSharedMemory();
void allocateSharedMemory();
void signalInterruption(int);

// global fields to track overall state
// would probably be better to not have these as globals, but it makes things
// much easier
int numProcesses = 0;
pid_t* childPids = NULL;
char* processName;

// globally store the shmid of shared memory once we've allocated it because we want
// to be easily able to deallocate it
int shmidTurn;
int shmidFlags;
int shmidNumProcesses;
int shmidBufferFlag1;
int shmidBuffer1;
int shmidBufferFlag2;
int shmidBuffer2;
int shmidBufferFlag3;
int shmidBuffer3;
int shmidBufferFlag4;
int shmidBuffer4;
int shmidBufferFlag5;
int shmidBuffer5;

int main(int argc, char** argv)
{
	int helpFlag = 0;
	int numChildren = 10;
	int n = 10;
	processName = argv[0];

	signal(SIGINT, signalInterruption);
	signal(SIGALRM, signalInterruption);
	
	// Overwrite file
	FILE* f = fopen("master.log", "w"); 
	fclose(f);
	
	int c;
	char* arg;
	while ((c = getopt(argc, argv, "hn:")) != -1)
	{
		switch (c)
		{
			case 'h':
				helpFlag = 1;
				displayHelp();
				break;
			case 'n':
				arg = optarg;
				if (checkNumber(arg))
				{
					n = atoi(arg);
				}
				break;
			default:
				n = 10;
				break;
		}
	}

	if (n > 0 && !helpFlag)
	{
		// can't spawn more than 20 children processes, so we need to limit this master from spawning more than 18
		// more processes as this will hit the limit
		if (n > 18)
		{
			n = 18;
		}
		allocateSharedMemory(n);

		childPids = malloc(sizeof(pid_t) * n);
		
		// create a child producer
		childPids[numProcesses++] = createChildProcess("./producer", processName);
		--n;

		if (n > 0)
		{
			int i;
			for (i = 0; i < n; ++i)
			{
				char processToCall[100];
				snprintf(processToCall, sizeof(processToCall), "./consumer %d", i + 1);
				childPids[numProcesses++] = createChildProcess(processToCall, processName);
			}
		}
	}

	setPeriodic(100);
	
	pid_t childpid;
	int status;
	while((childpid = wait(&status)) > 0)
		--numProcesses;
	
	if (childPids != NULL)
	{
		free(childPids);
		childPids = NULL;
	}
	
	deallocateAllSharedMemory();	
	return 0;
}

void allocateSharedMemory(int numChildren)
{
	key_t kTurn, kFlags, kNumProcesses,  kBufferFlag1, kBuffer1, kBufferFlag2, kBuffer2, kBufferFlag3, kBuffer3, kBufferFlag4, kBuffer4, kBufferFlag5, kBuffer5;

	const int memFlags = (0777 | IPC_CREAT);	
	//generate keys for shared memory variables
	kTurn = getKey(1);
	if ((shmidTurn = shmget(kTurn, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocated shared memory for key", processName);
	}
	
	kFlags = getKey(2);
	if((shmidFlags = shmget(kFlags, sizeof(int) * numChildren, memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for flags", processName);
	}
	
	kNumProcesses = getKey(3);
	if((shmidNumProcesses = shmget(kNumProcesses, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for flags", processName);
	}
	else
	{
		int* sharedNumProcesses = shmat(shmidNumProcesses, 0, 0);
		if ((int)sharedNumProcesses != -1)
		{
			*sharedNumProcesses = numChildren;
		}
	}
	
	kBufferFlag1 = getKey(4);
	if((shmidBufferFlag1 = shmget(kBufferFlag1, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 1 flag", processName);
	}

	kBuffer1 = getKey(5);
	if((shmidBuffer1 = shmget(kBuffer1, sizeof(BUFFER_SIZE), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 1", processName);
	}

	kBufferFlag2 = getKey(6);
	if((shmidBufferFlag2 = shmget(kBufferFlag2, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 2 flag", processName);
	}

	kBuffer2 = getKey(7);
	if((shmidBuffer2 = shmget(kBuffer2, sizeof(BUFFER_SIZE), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 2", processName);
	}

	kBufferFlag3 = getKey(8);
	if((shmidBufferFlag3 = shmget(kBufferFlag3, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 3 flag", processName);
	}

	kBuffer3 = getKey(9);
	if((shmidBuffer3 = shmget(kBuffer3, sizeof(BUFFER_SIZE), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 3", processName);
	}

	kBufferFlag4 = getKey(10);
	if((shmidBufferFlag4 = shmget(kBufferFlag4, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 4 flag", processName);
	}

	kBuffer4 = getKey(11);
	if((shmidBuffer4 = shmget(kBuffer4, sizeof(BUFFER_SIZE), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 4", processName);
	}

	kBufferFlag5 = getKey(14);
	if((shmidBufferFlag5 = shmget(kBufferFlag5, sizeof(int), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 5 flag", processName);
	}

	kBuffer5 = getKey(13);
	if((shmidBuffer5 = shmget(kBuffer5, sizeof(BUFFER_SIZE), memFlags)) == -1)
	{
		writeError("Failed to allocate shared memory for buffer 5", processName);
	}
}

void deallocateAllSharedMemory()
{
	deallocateSharedMemory(shmidTurn, processName);
	deallocateSharedMemory(shmidFlags, processName);
	deallocateSharedMemory(shmidNumProcesses, processName);
	deallocateSharedMemory(shmidBufferFlag1, processName);
	deallocateSharedMemory(shmidBuffer1, processName);
	deallocateSharedMemory(shmidBufferFlag2, processName);
	deallocateSharedMemory(shmidBuffer2, processName);
	deallocateSharedMemory(shmidBufferFlag3, processName);
	deallocateSharedMemory(shmidBuffer3, processName);
	deallocateSharedMemory(shmidBufferFlag4, processName);
	deallocateSharedMemory(shmidBuffer4, processName);
	deallocateSharedMemory(shmidBufferFlag5, processName);
	deallocateSharedMemory(shmidBuffer5, processName);
}

void displayHelp()
{
	printf("master: Application to spawn off child producer and consumer(s)\nOptions:\n-n Number of child processes to spawn\n");
}


void signalInterruption(int signo)
{
	if (signo == SIGINT || signo == SIGALRM)
	{
		if (signo == SIGINT)
			printf("Process interrupted by user\n");
		else if (signo == SIGALRM)
			printf("Process ended by timer\n");
		int i;
		for (i = 0; i < numProcesses; ++i)
			kill(childPids[i], SIGTERM);
		
		int status;
		pid_t childpid;		
		while((childpid = wait(&status)) > 0)
			--numProcesses;
		deallocateAllSharedMemory();
		
		if (childPids != NULL)
			free(childPids);
	
		exit(0);
	}	
}
