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
#include<string.h>
#include<time.h>
#include<signal.h>
#include"buffer_size.h"
#include"function_library.h"

enum state { idle = 0, want_in = 1, in_cs = 2};
void process(const int, const int, int*, int*, int*, char*, int*, char*, int*, char*, int*, char*, int*, char*);
void handleTerm(int);

char fileName[100];

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
	if (argc > 0)
		turnNumber = atoi(argv[1]);
	
	snprintf(fileName, 100, "cons%d.log", turnNumber);
	process(turnNumber, *numProcesses, turn, flags, bufferFlag1, buffer1, bufferFlag2, buffer2, bufferFlag3, buffer3, bufferFlag4, buffer4, bufferFlag5, buffer5);

	return 0;
}

void process(const int i, const int n, int* turn, int* flag, int* bufferFlag1, char* buffer1, int* bufferFlag2, char* buffer2,
	int* bufferFlag3, char* buffer3, int* bufferFlag4, char* buffer4, int* bufferFlag5, char* buffer5)
{
	FILE* consLog;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	consLog = fopen(fileName, "w");
	fprintf(consLog, "%02d:%02d:%02d\tStarted\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(consLog);
	
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
		
		char readBuf[BUFFER_SIZE];
		int checked = 0;
		if (*bufferFlag1 == 1)
		{
			strcpy(readBuf, buffer1);
			*bufferFlag1 = 0;	
			checked = 1;			
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tRead\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 1, readBuf);
			fclose(consLog);
		}
		else if (*bufferFlag2 == 1)
		{
			strcpy(readBuf, buffer2);
			*bufferFlag2 = 0;
			checked = 1;		
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tRead\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 2, readBuf);
			fclose(consLog);
		}
		else if (*bufferFlag3 == 1)
		{
			strcpy(readBuf, buffer3);
			*bufferFlag3 = 0;
			checked = 1;
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tRead\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 3, readBuf);
			fclose(consLog);
		}
		else if (*bufferFlag4 == 1)
		{
			strcpy(readBuf, buffer4);
			*bufferFlag4 = 0;
			checked = 1;	
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tRead\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 4, readBuf);
			fclose(consLog);
		}
		else if (*bufferFlag5 == 1)
		{
			strcpy(readBuf, buffer5);
			*bufferFlag5 = 0;
			checked = 1;	
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tRead\t%d\t%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, 4, readBuf);
			fclose(consLog);

		}

		if (checked == 0)
		{
			t = time(NULL);
			struct tm tm = *localtime(&t);
			consLog = fopen(fileName, "a");
			fprintf(consLog, "%02d:%02d:%02d\tCheck\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
			fclose(consLog);
		}
		
		//Exit section
		j = (*turn + 1) % n;
		while (flag[j] == idle)
		{
			j = (j + 1) % n;
		}
		
		//Assign turn to next waiting process; change own flag to idle 
		*turn = j; flag[i] = idle;
		int secondsToWait = rand() % 5;
		t = time(NULL);
		struct tm tm = *localtime(&t);
		consLog = fopen(fileName, "a");
		fprintf("%02d:%02d:%02d\tSleep\t%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, secondsToWait);
		sleep(secondsToWait);
	}while(1);
	
	consLog = fprintf(fileName, "a");
 	fprintf(consLog, "%d:%d:%d\tTerminated\tNormal\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(consLog);
}

void handleTerm(int signo)
{
	if (signo == SIGTERM)
	{
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		FILE* consLog = fopen(fileName, "a");
		fprintf(consLog, "%02d:%02d:%02d\tTerminated\tKilled\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(consLog);
		fprintf(stderr, "Consumer killed by master\n");
		exit(0);
	}
}
