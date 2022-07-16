#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h> 
#include "list.h"
int writes = 0;
int reads = 0;
int faults = 0;
int main(int argc, char *argv[])
{
	// Arguments are as such:
	// Algorith:0 for LRU or 1 for Second Chance
	// Size of memory
	// Queue length for each Process
	// Total Reading Loops
	char *conversionPtr, *line;
	double pageBytes = 4096.0; // Size of the Page
	int algorithm = -1, memory = -1, queue = -1, loopLimit = -1, globalClock = 0, loopCounter = 0, loopFlag = 1, totalTraces = 0, flag1 = 1, flag2 = 1, isWrite, i, pageNum, shift = log(pageBytes);
	size_t readSize = 20;
	struct memory **frameTable;
	
	// #######
	// Set the arguments
	if (argc != 7 && argc != 9)
	{
		// If the user provided bad input, give usage instructions
		printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
		exit(-1);
	}
	else if (argc == 9)
	{
		for (int i = 1; i < 9; i += 2)
		{
			if (strcmp("-a", argv[i]) == 0)
			{
				if (algorithm == -1)
				{
					algorithm = atoi(argv[i + 1]);
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else if (strcmp("-s", argv[i]) == 0)
			{
				if (memory == -1)
				{
					memory = atoi(argv[i + 1]);
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else if (strcmp("-q", argv[i]) == 0)
			{
				if (queue == -1)
				{
					queue = atoi((argv[i + 1]));
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else if (strcmp("-l", argv[i]) == 0)
			{
				if (loopLimit == -1)
				{
					loopLimit = atoi((argv[i + 1]));
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else
			{
				printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
				exit(-1);
			}
		}
	}
	else if (argc == 7)
	{
		for (int i = 1; i < 7; i += 2)
		{
			if (strcmp("-a", argv[i]) == 0)
			{
				if (algorithm == -1)
				{
					algorithm = atoi(argv[i + 1]);
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else if (strcmp("-s", argv[i]) == 0)
			{
				if (memory == -1)
				{
					memory = atoi(argv[i + 1]);
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else if (strcmp("-q", argv[i]) == 0)
			{
				if (queue == -1)
				{
					queue = atoi((argv[i + 1]));
				}
				else
				{
					printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
					exit(-1);
				}
			}
			else
			{
				printf("Usage: ./sim -a <algorithm> -s <memory_size> -q <queue_size> -l <(optional)loops>\n");
				exit(-1);
			}
		}
		loopLimit = 10;
	}
	// #######

	if (argc > 4)
	{ // We set the -Optional- Limit of traces per proccess
		loopLimit = strtol(argv[4], &conversionPtr, 10) / queue;
		loopFlag = 0;
	}
	printf("Algorithm (0=LRU 1=Second Chance): %d Frames: %d Traces per Proccess per Loop: %d Loops: %d ", algorithm, memory, queue, loopLimit);
	printf("Table size: %d\n", memory);
	frameTable = malloc(sizeof(struct memory *) * memory);
	FILE *file1, *file2; // We open the Files
	file1 = fopen("bzip.trace", "r");
	file2 = fopen("gcc.trace", "r");
	struct table *bzipTable, *gccTable; // We setup and initialize the hash Tables
	bzipTable = initTable(20000, 1);
	gccTable = initTable(30000, 2);
	while ((flag1 || flag2) && (loopCounter < loopLimit || loopFlag))
	{ // We read the specified number of traces
		for (i = 0; i < queue; i++)
		{
			globalClock++;
			line = NULL;
			if (getline(&line, &readSize, file1) < 0)
			{
				flag1 = 0;
				break;
			}
			pageNum = strtol(line, &conversionPtr, 16) >> shift; // Remove the offset
			if (conversionPtr[1] == 'R')
			{ // Check if it is Read or Write
				isWrite = 0;
			}
			else if (conversionPtr[1] == 'W')
			{
				isWrite = 1;
			}
			addToTable(pageNum, bzipTable, frameTable, memory, globalClock, isWrite, algorithm); // Add to the table
			totalTraces++;
		}
		for (i = 0; i < queue; i++)
		{ // Same as above
			globalClock++;
			line = NULL;
			if (getline(&line, &readSize, file2) < 0)
			{
				flag2 = 0;
				break;
			}
			pageNum = strtol(line, &conversionPtr, 16) >> shift;
			if (conversionPtr[1] == 'R')
			{
				isWrite = 0;
			}
			else if (conversionPtr[1] == 'W')
			{
				isWrite = 1;
			}
			addToTable(pageNum, gccTable, frameTable, memory, globalClock, isWrite, algorithm);
			totalTraces++;
		}
		loopCounter++;
	}
	printf("Writes: %d Reads: %d Faults: %d Total Traces: %d\n", writes, reads, faults, totalTraces);
	return 0;
}
