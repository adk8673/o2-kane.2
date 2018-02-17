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
#include"function_library.h"

void displayHelp();

int main(int argc, char** argv)
{
	int helpFlag = 0;
	int numChildren = 10;
	int n = 10;

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
		}
	}

	if (n > 0 && !helpFlag)
	{
		printf("%d\n", n);

		// can't spawn more than 20 children processes, so we need to limit this master from spawning more than 18
		// more processes as this will hit the limit
		if (n > 18)
		{
			n = 18;
		}

		// create a child proceder
		createChildProcess("./producer", NULL);
		--n;

		if (n > 0)
		{
			int i;
			for (i = 0; i < n; ++i)
			{
				createChildProcess("./consumer", NULL);
			}
		}
	}
	
	pid_t childpid;
	int status;
	while((childpid = wait(&status)) > 0);
	
	return 0;
}

void displayHelp()
{
	printf("master: Application to spawn off child producer and consumer(s)\nOptions:\n-n Number of child processes to spawn\n");
}
