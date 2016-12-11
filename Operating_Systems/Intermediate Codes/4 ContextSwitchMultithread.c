/* Context Switching with multiple threads + interrupt handling */

#include<stdio.h> //for printf
#include<pthread.h> //for threads, mutual exclusion, conditional wait and signalling
#include<time.h> //for accurate clock time
#include<limits.h> //for min-max limits of several data types
#include<inttypes.h> //for using uint64_t data type
#include<stdlib.h> //for malloc
//#include<semaphore.h>
#include <stdbool.h> //for boolean variables
#include<string.h> // for string operations
#include<signal.h> // for signalling
#define nothread 4

#define TIMESLICE 2000000000 //nanoseconds to define milliseconds
#define BILLION 1000000000L

void *threadFunction(void *arg);
void *dispatcher(void *arg);

/* declare catchint, later to be used as the handler */
void catchint(int);

struct Node{
	pthread_t t;
	int tid;
	struct Node *next;
}*startq,*rearq;

struct Node* createNode(int id)
{
	struct Node* a = malloc(sizeof(struct Node));
	a->tid=id;
	a->next=NULL;
	return a;
}

struct Node* pop()
{
	struct Node* temp=startq;
	if(startq==rearq)
		startq=rearq=NULL;
	else
		startq= startq->next;		

	return temp;
}

void push(struct Node* temp)
{
	if(startq==NULL)
		startq=rearq=temp;
	else
	{
		rearq->next=temp;
		rearq=temp;
	}
}


typedef struct ThreadControlBlock {
	bool blockedState;
	bool exitStatus;
	char *filePaths[3];
} TCB;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t tid[2];
pthread_t disp; //dispatcher
uint64_t diff;
int currentThread;
struct timespec start, end;
int numProcWait = nothread; //No of processes running
TCB process[nothread];

void *threadFunction(void *arg) {
	int i,instructions = INT_MAX;
	bool locked,unlockCheck=true; // locked for synchronizing the thread withing itself
	char buff[100];
	uint64_t diff=0;

	TCB *process = (TCB *)arg;
	int fileCount = sizeof(process->filePaths)/sizeof(process->filePaths[0]);
	FILE *fp[fileCount];

	char *fileNames[fileCount];
	
	printf("\nWaiting to start Thread id: %lu\n",pthread_self());
	/*	
	while(process->blockedState);
	for(i = 0; i<fileCount; i++) {
		if(process->filePaths[i]!=NULL) {
			//printf("NOT NULL\n");
			fileNames[i] = malloc(sizeof(char) * strlen(process->filePaths[i]));
			strcpy(fileNames[i],process->filePaths[i]);
			printf("FILE NAME: %s\n",fileNames[i]);
			fp[i] = fopen(fileNames[i], "r");
		}
		else {
			fp[i] = NULL;
		}
	}*/
	printf("Received Signal Successful! Thread ID: %lu.\n",pthread_self());
	locked = true;

	char c = 0;
	for (i = INT_MIN; i < instructions; i++) {
		if(process->blockedState) {
			while(process->blockedState);
			printf("\nCurrent executing Thread ID: %lu.\n",pthread_self());
		}
		/*else {
			if((c = fgetc(fp[0])) != EOF) {
				//printf("%c",c);
				//printf(" ");
			}
			else {
				printf("\n\nBREAKING!\n");
				break;
			}
		}*/
	}
/*
	for(i = 0; i<fileCount; i++) {
		if(process->filePaths[i]!=NULL) {
			free(fileNames[i]);
			fclose(fp[i]);
		}
	}*/

	numProcWait--;
	process->exitStatus = true;
	printf("EXITING THREAD: %lu\n\n",pthread_self());
	printf("Number of processes waiting: %d\n\n",numProcWait);
	process->exitStatus = true;
	return NULL;
}

void *dispatcher(void *arg) {
	diff = 0;
	TCB *process = (TCB *) arg;
	//printf("STATE1: %s\n",*blockedState?"true":"false");
	//printf("STATE2: %s\n",*(blockedState+1)?"true":"false");

	clock_gettime(CLOCK_MONOTONIC, &start);
	(process -1 + startq->tid)->blockedState = false;
	while(numProcWait>0) {

		clock_gettime(CLOCK_MONOTONIC, &end);
		diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
		if(diff > TIMESLICE) {
			//if(numProcWait>1) {
				pthread_mutex_lock(&lock);
				//printf("%d\n",startq->tid);
				(process -1 + startq->tid)->blockedState = !(process -1 + startq->tid)->blockedState;
				struct Node *curr=pop();
				if((process -1 + startq->tid)->exitStatus==true)
					pop();
				(process -1 + startq->tid)->blockedState = !(process -1 + startq->tid)->blockedState;
				push(curr);
			//}
				clock_gettime(CLOCK_MONOTONIC, &start);
				pthread_mutex_unlock(&lock);
		}
	}
}

void catchint(int signo) {
	pthread_mutex_lock(&lock);
	printf("\nInterrupt Subrouting: CHANGING THREAD!\n");
	(process -1 + startq->tid)->blockedState = !(process -1 + startq->tid)->blockedState;
	struct Node *curr=pop();
	if((process -1 + startq->tid)->exitStatus==true)
					pop();
	(process -1 + startq->tid)->blockedState = !(process -1 + startq->tid)->blockedState;
	push(curr);	
	clock_gettime(CLOCK_MONOTONIC, &start);
	pthread_mutex_unlock(&lock);
}

int main() {

	startq=rearq=NULL;
	
	for(int i=0;i<nothread;++i)
	{
		struct Node* temp = createNode(i+1);
		push(temp);			
	}
	struct timespec startfinal, endfinal;

	for(int i=0;i<nothread;++i)
	{
		process[i].blockedState = true;
		process[i].blockedState = true;
		process[i].exitStatus = false;
		process[i].exitStatus = false;
	}
	/*
	process[0].filePaths[0] = "/home/bhavya/bhavya/Github/OS/Project/file1.txt";
	process[0].filePaths[1] = NULL;
	process[0].filePaths[2] = NULL;
	process[1].filePaths[0] = "/home/bhavya/bhavya/Github/OS/Project/file2.txt";
	process[1].filePaths[1] = NULL;
	process[1].filePaths[2] = NULL;
	*/
	clock_gettime(CLOCK_MONOTONIC, &startfinal);

	static struct sigaction act;
	struct Node *iter=startq; 
    /* set up the action to be taken on receipt of SIGNINT */
    act.sa_handler = catchint;

    /*create a full mask */
    sigfillset(&(act.sa_mask));

    /*before sigaction call, SIGINT will terminate process (defualt action) */
    sigaction(SIGINT, &act, NULL);
    /* on receipt of SIGINT control will be passed to catchint */
	
	pthread_create(&disp, NULL, dispatcher, &process);
	for(int i=0;i<nothread;++i,iter=iter->next)
	{
		pthread_create(&(iter->t), NULL, threadFunction, (void *)&process[iter->tid-1]);		
	}
		
	struct Node *iter2=startq;
	for(int i=0;i<nothread;++i,iter2=iter2->next)
	{
		pthread_join(iter2->t, NULL);
		printf("Thread %d EXITED. No. of Waiting Processes: %d\n",i+1,numProcWait);
	}

	clock_gettime(CLOCK_MONOTONIC, &endfinal);
	diff = BILLION * (endfinal.tv_sec - startfinal.tv_sec) + endfinal.tv_nsec - startfinal.tv_nsec;
	printf("Total Run Time: %" PRIu64 " ms\n",diff/1000000);

	pthread_mutex_destroy(&lock);
}
