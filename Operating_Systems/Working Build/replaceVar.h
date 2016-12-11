void replaceVal(char *instruction, int value)
{
	
	int len = strlen(instruction);
	FILE *re = fopen("resource.txt","r");
	char c=getc(re);
	int count=1;
	while(c!=EOF)
	{c=getc(re);count++;
	}
	rewind(re);
	char *buff = (char *)malloc(count*sizeof(char));
	int i=1,j=0,k=0,l=0;

	for(i = 0;i<count;i++)
		buff[i]=getc(re);
	fclose(re);

	for(i=0,j=0;j<count-1;j++)
	{

		if(buff[j]==instruction[i])
		{
			k++;i++;
			if(k==len && buff[j+1]==' ')
			{
				j-=(k-1);
				while(buff[j]!='\n' )
				{
					buff[j]='*';
					j++;
				}
				buff[j]='*';

			}
		}
		else
		{
			while(buff[j]!='\n' )
			j++;
			i=0;
			k=0;
		}
	}


	re = fopen("resource.txt","w");
	i=0;
	for(i=0;i<len;i++)
	fputc(instruction[i],re);
	fprintf(re," %d\n",value);

	i=0;
	c=buff[i];
	while(c!=EOF)
	{
		while(c=='*')
		{
			i++;
			c=buff[i];
		}
		fputc(c,re);
		i++;
		c=buff[i];
	}
	fclose(re);


}
