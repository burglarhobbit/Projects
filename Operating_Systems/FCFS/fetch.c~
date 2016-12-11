#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"decode.h"
#include<pthread.h>

int fetch(char *fileIn,int processID)
{
	FILE *fp= fopen(fileIn,"r");
	char ch='0',*tmp;
	int insSize=0;
	pthread_t insExe;int insStatus;
	
	while (ch != EOF)
	{	
		while(ch!='\n'){
			ch = getc(fp);	
			insSize++;	
		}
		fseek(fp,-(insSize),SEEK_CUR);
		tmp = (char *)malloc(insSize*sizeof(char));
		fread(tmp,insSize,sizeof(char),fp);
		pri((void *)tmp,processID);
		ch = getc(fp);
		insSize=1;
			
	}
	fclose(fp);
  return 0;
}
struct queue{
	char *filenm;
	struct queue *q;
};
int main()
{

	int i=0;
	struct queue *proQ=NULL,*head=NULL;
	proQ=(struct queue *)malloc(sizeof(struct queue));
	head=proQ;
	proQ->filenm="filein_1.txt";
	proQ->q = (struct queue *)malloc(sizeof(struct queue));
	proQ = proQ->q;
	proQ->filenm="filein_2.txt";
	proQ->q = (struct queue *)malloc(sizeof(struct queue));
	proQ = proQ->q;
	proQ->filenm="filein_3.txt";
	proQ->q = (struct queue *)malloc(sizeof(struct queue));
	proQ = proQ->q;
	proQ->filenm="filein_4.txt";
	proQ->q = (struct queue *)malloc(sizeof(struct queue));
	proQ = proQ->q;
	proQ->filenm="filein_5.txt";
	proQ->q = NULL;



	time_t t;
    	time(&t);


	int n,bt[20],wt[20],tat[20],avwt=0,avtat=0,j;//variables used for Burst Time, Wait Time, turnaround time and Throughput
	n=5;//number of processes. Here there are 5 processes but currently we are working with 3 processes.	
	char start_time[6][30];	
	char end_time[6][30];

	//Note: We have taken arrival time to be 0 for all the process and at that time process has been arrived...
	//... in the following order : 1,2,3,4,5 

	//Assigning burst time. Burst time is the service time required for the processes.
	bt[0]=20;bt[1]=2;bt[2]=15;bt[3]=25;bt[4]=35;


	time_t result;
	
	for(i=0;i<5;i++){
	result=time(NULL);
	sprintf(start_time[i],"%s",asctime(localtime(&result))); 
	fetch(head->filenm,(i+1));
	result=time(NULL);
	sprintf(end_time[i],"%s",asctime(localtime(&result))); 
	head=head->q;
	}
 
//*****************************Generating Process Log - Ends ***********************************************
   wt[0]=0;    //waiting time for first process is 0
 
    //calculating waiting time
    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++)
            wt[i]+=bt[j];
    }
 
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time");
 
    //calculating turnaround time
    for(i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];
        avwt+=wt[i];
        avtat+=tat[i];
        printf("\n P[%d] \t \t %d \t \t% d \t \t%d",i+1,bt[i],wt[i],tat[i]);
    }
 
    avwt/=i;
    avtat/=i;
    printf("\n\nAverage Waiting Time:%d",avwt);
    printf("\nAverage Turnaround Time:%d\n",avtat);


//FILE
   FILE *fiptr;
   fiptr=fopen("fcfs_process_log.txt","a+");
   if(fiptr==NULL){
      printf("Error!");
      exit(1);
   }

else{



    for(i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];
        avwt+=wt[i];
        avtat+=tat[i];
        fprintf(fiptr,"\n P[%d] \t \t %d \t \t%d \t \t%d \t\t %s \t\t\t\t\t\t\t\t %s",i+1,bt[i],wt[i],tat[i],start_time[i],end_time[i]);
    }

}
   fclose(fiptr);

    return 0;
}
