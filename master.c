// master.c
// Code for master executable which will be used to spawn producer and consumer children as well as handle
// signals
// Alex Kane
// 2/10/18
// CS 4760 Project 2
#include<unistd.h>
#include"function_library.h"

int main(int argc, char** argv)
{
	int helpFlag = 0;
	int numChildren = 10;
	int c;
	while ((c = getopt(arc, argv, "hn:") != -1)
	{
		switch (c)
		{
			case 'h':
				displayHelp();
				break;
			case 'n':
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

		// create a child proceder
		createChildProcess("./producer", NULL);

		--n;

		if (n > 0)
		{
			int i;
			for (i = 0; i < n; ++i)
				createChildProcess("./consumer", NULL);
		}
	}

	
	return 0;
}

void displayHelp()
{

}
