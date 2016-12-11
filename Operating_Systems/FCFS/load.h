#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int load(char *filenm,char *resource_name, int lengther) {
	FILE *fp = fopen(filenm, "r");
	char ch[2],*tmp;
	int insSize=0;
	ch[0] ='0' ;
	int looper;
	int var_length;
	int success=0;
	int answer;

	while (ch[0] != EOF) {
		while(ch[0]!='\n') {
			ch[0] = getc(fp);	
			insSize++;	
		}
		fseek(fp,-(insSize),SEEK_CUR);
		tmp = (char *)malloc(insSize*sizeof(char));
		fread(tmp,insSize,sizeof(char),fp);
		int length=sizeof(tmp);
		int i = 0;

		while(tmp[i]!=32) {	
			i++;
		}
		var_length=i;
		
		success=0;
		for(i=0;i<lengther;i++) {
			if(resource_name[i]==tmp[i]) {
				success++;		
			}
		}

	
		if(success==lengther && success==var_length) {
			answer=atoi(&tmp[var_length+1]);	
		}

		ch[0] = getc(fp);
		insSize=1;
	}
	fclose(fp);		
	return answer;
}
