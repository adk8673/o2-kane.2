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
#include <string.h>
#include <time.h>
#include <signal.h>
#include "function_library.h"

void process(const int, const int, int*, int*, int*, char*, int*, char*, int*, char*, int*, char*, int*, char*);
enum state { idle = 0, want_in = 1, in_cs = 2};
void handleTerm(int);

int main(int argc, char** argv)
{
	signal(SIGTERM, handleTerm);
	srand(time(NULL));
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
	int* bufferFlag5 = getExistingSharedMemory(14, argv[0]);
	char* buffer5 = getExistingSharedMemory(13, argv[0]);

	int turnNumber = 0;
	process(0, *numProcesses, turn, flags, bufferFlag1, buffer1, bufferFlag2, buffer2, bufferFlag3, buffer3, bufferFlag4, buffer4, bufferFlag5, buffer5);
	return 0;
}

void process(const int i, const int n, int* turn, int* flag, int* bufferFlag1, char* buffer1, int* bufferFlag2, char* buffer2,
	int* bufferFlag3, char* buffer3, int* bufferFlag4, char* buffer4, int* bufferFlag5, char* buffer5)
{
	FILE* f = fopen("testdata.txt", "r");
	FILE* prodLog;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	prodLog = fopen("prod.log", "w");
	fprintf(prodLog, "%02d:%02d:%02d\tStarted\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(prodLog);	

	int j;
	do 
	{
		do
		{
			flag[i] = (int)want_in;
			j = *turn;
			while (j != i)
				j = (flag[j] != idle) ? *turn : (j + 1) % n;

			// declare intention to enter critical section
			flag[i] = in_cs;

			// check that no one else is in the critical section
			for (j = 0; j < n; j++)
				if ((j != i) && (flag[j] == in_cs))
					break;
		} while ((j < n) || (*turn != i && flag[*turn] != idle));
		
		// Assign turn to self and enter critical section
		*turn = i;
		char readBuf[500];
		int wrote = 0;
		if (*bufferFlag1 == 0)
		{
			printf("test2\n");
			fgets(readBuf, 500, f);
			strcpy(buffer1, readBuf);
			*bufferFlag1 = 1;
			t = time(NULL);
			tm = *localtime(&t);
			prodLog = fopen("prod.log","a");
			fprintf(prodLog, "%02d:%02d:%02d\tWrite\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 1, buffer1);
			fclose(prodLog);
			wrote = 1;
		}
		if (*bufferFlag2 == 0)
		{
			fgets(readBuf, 500, f);
			strcpy(buffer2, readBuf);
			*bufferFlag2 = 1;
			t = time(NULL);
			tm = *localtime(&t);
			prodLog = fopen("prod.log","a");
			fprintf(prodLog, "%02d:%02d:%02d\tWrite\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 2, buffer2);
			fclose(prodLog);
			wrote = 1;
		}
		if (*bufferFlag3 == 0)
		{
			fgets(readBuf, 500, f);
			strcpy(buffer3, readBuf);
			*bufferFlag3 = 1;
			t = time(NULL);
			tm = *localtime(&t);
			prodLog = fopen("prod.log", "a");
			fprintf(prodLog, "%02d:%02d:%02d\tWrite\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 3, buffer3);
			fclose(prodLog);
			wrote = 1;
		}
		if (*bufferFlag4 == 0)
		{
			fgets(readBuf, 500, f);
			strcpy(buffer4, readBuf);
			*bufferFlag4 = 1;
			t = time(NULL);
			tm = *localtime(&t);
			prodLog = fopen("prod.log", "a");
			fprintf(prodLog, "%02d:%02d:%02d\tWrite\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 4, buffer4);
			fclose(prodLog);
			wrote = 1;
		}
/*		else if (*bufferFlag5 == 0)
		{
			fgets(readBuf, 500, f);
			strcpy(buffer5, readBuf);
			*bufferFlag5 = 1;
		}*/
	

		if (wrote == 0)
		{
			prodLog = fopen("prod.log", "a");
			fprintf(prodLog, "%02d:%02d:%02d\tCheck\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
			fclose(prodLog);
		}
		
		//Exit section
		j = (*turn + 1) % n;
		while (flag[j] == idle)
		{
			j = (j + 1) % n;
		}
		
		//Assign turn to next waiting process; change own flag to idle 
		*turn = j; flag[i] = idle;
		sleep(rand() % 10);
	}while(1);
 	fclose(f);
	
	t = time(NULL);
	tm = *localtime(&t);
	prodLog = fopen("prod.log", "a");
	fprintf(prodLog, "%02d:%02d:%02d\tTerminated\tNormal\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(prodLog);
}

void handleTerm(int signo)
{
	if (signo == SIGTERM)
	{
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		FILE* prodLog = fopen("prod.log", "a");
		fprintf(prodLog, "%02d:%02d:%02d\tTerminated\tKilled\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(prodLog);
		fprintf(stderr, "Producer killed by master\n");
		exit(0);
	}
}
