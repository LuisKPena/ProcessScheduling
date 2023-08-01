/*
 * This is a program that reads input from a file containing
 * the burst time and arrival times of different processes,
 * then simulates different scheduling algorithms per the user's
 * intructions. The two algorithms available are First Come First
 * Serve (FCFS) and Shortest Job First (SFJ).
 *
 * @author Luis Pena (PID 6300130)
 * @version 06/03/2023
 *
 * I, Luis K. Pena certify that this is my own original program 
 *developed by me with no internet or outside help.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 200

typedef struct
{
	int pid;
	int arrivalTime;
	int burstTime;

} PROCESS;

//Function to perform First Come First Serve scheduling algorithm
void fcfs(PROCESS processes[], int numProcesses)
{
	//Intializing and instantiating variables
	int completionTime[MAX];
	int waitingTime[MAX];
	int turnaroundTime[MAX];
	int waitTime;
	float totalWaitTime = 0;
	float totalTurnaroundTime = 0;
	float avgWaitTime;
	float avgTurnaroundTime;

	//Setting initial status of variables
	waitingTime[0] = 0;
	turnaroundTime[0] = processes[0].burstTime;
	completionTime[0] = processes[0].burstTime;

	//Iterating over each process
	for(int i = 1; i < numProcesses; i++)
	{
		//Calculating wait time for each process
		waitTime = completionTime[i - 1] - processes[i].arrivalTime;

		//Setting wait time if greater than 0
		if(waitTime > 0)
		{
			waitingTime[i] = waitTime;
		}

		else
		{
			waitingTime[i] = 0;
		}

		//Calculating turnaround time for each process
		turnaroundTime[i] = waitingTime[i] + processes[i].burstTime;
		
		//Calculating completion time for each process
		completionTime[i] = processes[i].arrivalTime + turnaroundTime[i];
	}


	//Calculating total wait and turnaround time for all processes
	for(int i = 0; i < numProcesses; i++)
	{
		totalWaitTime += waitingTime[i];
		totalTurnaroundTime += turnaroundTime[i];
	}

	//Calculating average wait and turnaround times for all processes
	avgWaitTime = totalWaitTime/numProcesses;
	avgTurnaroundTime = totalTurnaroundTime/numProcesses;

	//Execution order label for FCFS
	printf("Execution Order for FCFS: ");

	//Printing execution order based on PID
	for(int i = 0; i < numProcesses; i++)
	{
		printf("P%d\t", processes[i].pid);
	}

	printf("\n");

	//Printing average wait and turnaround times for all processes
	printf("%s %.2f\n", "Average Wait Time:", avgWaitTime);
	printf("%s %.2f\n", "Average Turnaround Time:", avgTurnaroundTime);
}


//Function to perform Shortest Job First scheduling algorithm
void sjf(PROCESS processes[], int numProcesses)
{
	//Instantiating and intializing variables
	int completionTime[MAX];
	int waitingTime[MAX];
	int turnaroundTime[MAX];
	int completed[MAX] = {0};
	int currTime = 0;
	int complete = 0;
	int shortest = 0;
	int execOrder[MAX];
	int execInd = 0;
	float totalWaitTime = 0;
	float totalTurnaroundTime = 0;
	float avgWaitTime = 0;
	float avgTurnaroundTime = 0;

	//Creating main loop for SJF simulation
	while (complete != numProcesses)
	{
		//Setting default shortest process
		shortest = -1;

		//Calculating shortest job 
		for(int i = 0; i < numProcesses; i++)
		{
			//Checking that process has arrived and is incomplete
			if(processes[i].arrivalTime <= currTime && !completed[i])
			{
				//Checking if arriving process is first process or shortest
				if(shortest == -1 || processes[i].burstTime < processes[shortest].burstTime)
				{
					//If so, set shortest to current process
					shortest = i;
				}
			}
		}

		//Checking for valid shortest process
		if(shortest == -1)
		{
			//Moving scheduler to next unit
			currTime += 1;
			
			//Returning to loop
			continue;
		}

		//Updating completion, turnaround, and wait time
		completionTime[shortest] = currTime + processes[shortest].burstTime;
		turnaroundTime[shortest] = completionTime[shortest] - processes[shortest].arrivalTime;
		waitingTime[shortest] = turnaroundTime[shortest] - processes[shortest].burstTime;


		//Getting completed processes and incrementing process count
		completed[shortest] = 1;
		complete++;
		currTime = completionTime[shortest];

		//Setting order of execution
		execOrder[execInd++] = shortest;
	}

	//Calculating total wait and total turnaround time
	for(int i = 0; i < numProcesses; i++)
	{
		totalWaitTime += waitingTime[i];
		totalTurnaroundTime += turnaroundTime[i];
	}

	//Calculating average wait and average turnaround time
	avgWaitTime = totalWaitTime/numProcesses;
	avgTurnaroundTime = totalTurnaroundTime/numProcesses;

	//Execution order label for SJF
	printf("Execution Order for SJF: ");

	//Printing order of execution for processes
	for(int i = 0; i < numProcesses; i++)
	{
		printf("P%d ", processes[execOrder[i]].pid);
	}

	printf("\n");

	//Printing average wait and average turnaround time
	printf("%s %.2f\n", "Average Wait Time:", avgWaitTime);
	printf("%s %.2f\n", "Average Turnaround Time:", avgTurnaroundTime);
}


//Main function
int main(int argc, char **argv)
{
	//Instantiating and initializing variables
	extern char *optarg;
	extern int optind;
	int err = 0, numProcesses;
	PROCESS processes[MAX];
	FILE *inputFile;
	char inputFileName[20], schedulerAlgo[10]; 
	static char usage[] = "usage: %s input_file_name [name ...] scheduler algorithm [algo ...] \n";

	//Checking that an input file and scheduler algorithm are given  
	if((optind + 2) > argc)
	{	
		//Informing user an input file/scheduler algorithm are required
		fprintf(stderr, "%s: missing input file name and/or scheduler algorithm\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}	
	
	//Accepting arguments from command line
	if(optind < argc)
	{	
		//Setting input file name from command line
		strcpy(inputFileName, argv[optind]);
		
		//Opening input file in Read mode
		inputFile  = fopen(inputFileName,"r");
		
		//Informing user if file cannot be found/opened
		if(!inputFile)
		{
			printf("Could not open input file %s\n", inputFileName);
			exit (101);
		}
		
		//Passing first line in file as number of processes argument
		fscanf(inputFile, "%d", &numProcesses);

		//Passing rest of input file data to PROCESS struct
		for(int i = 0; i < numProcesses; i++)
		{
			//Setting each burst time and arrival time for processes
			fscanf(inputFile, "%d %d", &processes[i].burstTime, &processes[i].arrivalTime);
			
			//Generating process ID
			processes[i].pid = i+1;
		}

		//Closing input file
		fclose(inputFile);

		//Setting scheduler algorithm from command line
		strcpy(schedulerAlgo, argv[optind + 1]);

		//Passing input file data to FCFS function
		if(strcmp(schedulerAlgo, "FCFS") == 0)
		{
			fcfs(processes, numProcesses);		
		}

		//Passing input file data to SJF function
		if(strcmp(schedulerAlgo, "SJF") == 0)
		{
			sjf(processes, numProcesses);
		}
	}

	//Informing user if there are no arguments left
	else
	{
		printf("no arguments left to process\n");
	}

	//Returning 0 when program is finished
	return 0;
}
