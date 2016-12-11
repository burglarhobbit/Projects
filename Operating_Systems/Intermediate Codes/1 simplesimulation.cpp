#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#define timeslice 6

using namespace std;

int time=0;

struct PCB
{
	int pid;
	int beg;
	int term;	
	PCB* nextready;
	PCB* nextblocked;
} *newptr,*top,*begin,*end;

PCB* initPCB(int prid,int b,int t);
void push(PCB* ptr);
PCB* pop();
void display();
void process(PCB* ptr);

int main()
{
	top=NULL;
	for(int i=1; i<=10;++i)
	{	
		newptr=initPCB(i,i*1000,(i*1000+i*1));
		push(newptr);
	}	
	
	while(begin!=NULL)
	{	
		newptr=pop();
		process(newptr);
	}	
	
	return 0;
} 

PCB* initPCB(int prid,int b,int t)
{
	PCB* ptr = new PCB;
	ptr->pid=prid;
	ptr->beg=b;
	ptr->term=t;
	ptr->nextready=NULL;
	ptr->nextblocked=NULL;
	return ptr;
} 

void push(PCB* ptr)
{
	if(begin==NULL)
		begin=end=ptr;
	else
	{
		end->nextready=ptr;
		end=ptr;
	}	
}


void display()
{
	for(PCB* temp =begin;  temp!=NULL; temp=temp->nextready)
	{
		cout<<temp->pid<<" "<<temp->beg<<endl;
	}	
}


PCB* pop()
{
	if(begin!=NULL)
	{
		PCB* temp=begin;
		begin=begin->nextready;
		temp->nextready=NULL;
		return temp;
	}	

}

void process(PCB* ptr)
{
	int flag=0;
	for(int i=0;i<timeslice;++i)
	{
		if(ptr->beg==ptr->term)
		{
			cout<<ptr->pid<<" "<<ptr->beg<<" "<<time<<endl;
			flag=1;
			break;
		}
		else
		{
			ptr->beg++;
		}	
		time++;
	}	
	
	if(flag==0)
	{
		cout<<ptr->pid<<" "<<ptr->beg<<" "<<time<<endl;
		if(top==NULL)
			top=ptr;
		else if(flag==0)
		{
			push(ptr);
		}	
		else	
		{
			PCB* temp=top;
			top=ptr;
			ptr->nextblocked=temp;
		}	
	}	
}
