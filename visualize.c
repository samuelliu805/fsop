#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "visualize.h"
#include "parse.h"

void visualize(FILE * outfp)
{
	fprintf(outfp,"\n------------Visualized Output------------\n");
	int i;
	int j;
	int k=0;
	//char toterminal[MAX_FILES_DETAIL];
	//int level=0;

	/*
	//some bugs 
	for (i=0; i<MAX_DETAIL_FILES; i++)
		recursive(&detail[i],0,0,1);
		*/


	//char toterminal[MAX_FILES_DETAIL];
	/*
	char test[50];
	memset(test,'\0',50);
	memcpy(test,"/lib/x86_64-linux-gnu/libc.so.6",40);
	test="/lib/x86_64-linux-gnu/libc.so.6";
	printf("%s\n\n",test);
	while (isenddir(test,k)==false)
	{
			getdirname(test,toterminal,k);
			printf("%s\n",toterminal);
			k++;

	}

	*/


	for (i=0; i<MAX_FILES_DETAIL; i++)
	{
		if (detail[i].start==true)
		{
			fprintf(outfp,"%s\n",detail[i].filename);
			for (j=0; j<MAX_FILES_DETAIL ; j++)
			{
				if (j!=i && detail[j].start==true && issamedir(detail[j].filename,detail[i].filename,0))
				{
					//printf("----");
					for (k=0; k<MAX_FILES_DETAIL; k++)
					{
						if (j!=k && detail[k].start==true && issamedir(detail[j].filename,detail[k].filename,0))
						{
							//printf("----%s\n", detail[k].filename);
							printlevel(detail[k].filename,2);
							detail[k].start=false;
				
						}
					
					}
				//	printf("%s\n", detail[j].filename);
					//detail[j].start=false;
				}
			}
		}
	}




}


//to get real directory listing visualization, a recursive part needed to be done, unfortunlately i dont have time to finish it
void recursive(file_details* current, int level, int start, int root)
{

	if (current->start==false)
		return;
	//printf("%d\n %s \n\n",level,current->filename);

	char toterminal[MAX_FILES_DETAIL];
	getdirname(current->filename,toterminal,level);
	if (isenddir(current->filename,level)==true)
	{

		printlevel(toterminal,level);
		current->start=false;
		return;
	}
	if (root==1)
	{
		printlevel(toterminal,level);
	}
	int i;
	//int curr;
	for (i=start; i<MAX_DETAIL_FILES; i++)
	{
		if (&detail[i]!=current && detail[i].start==true)
		{
			if (issamedir(detail[i].filename,current->filename,level)==true)
				recursive(&detail[i],level+1, i+1,0);

		}
	}	

	//recursive(&detail[curr+1],0);

}

void printlevel(char* string, int level)
{
	int i=0;
	printf("\n");
	for (i=0; i<level; i++)
		printf("----");
	printf("%s",string);


}

bool issamedir(char* name, char* target, int level)
{

	char ret[MAX_FILES_DETAIL];
	char ret1[MAX_FILES_DETAIL];
	getdirname(name, ret, level);
	getdirname(target,ret1,level);
	if (strncmp(ret,ret1,MAX_FILES_DETAIL)==0)
		return true;
	return false;

}

void getdirname(char* name, char* ret, int level)
{

	int i=1;
	int count=0;
	int bytes=0;
	int start=0;
		//printf("\n beforememset: %s\n", ret);

	memset(ret,'\0',MAX_FILES_DETAIL);
	//printf("original file:%s\n level: %d\n",name,level);
	//printf("aftermemset: %s\n", ret);
	while (name[i]!='\0')
	{
		//printf("2");
		if (i>=MAX_FILES_DETAIL)
			break;
		bytes++;
		if (name[i]=='/')
		{
			if (count==level)
			{
				memcpy(ret,&name[start], bytes+1);
				//if (level!=0)
		//			printf("ret is: %s, %d",ret,bytes);
				return;
			}
			start=i;
			bytes=0;
			count++;
		}
		i++;
	}
	memcpy(ret,name,bytes+1);
					//if (level!=0)

				//printf("ret is: %s, %d",ret,bytes);


}

bool isenddir(char* name, int level)
{
	int i=1;
	int count=0;

	while (name[i]!='\0')
	{
				//printf("2");

		if (i>=MAX_FILES_DETAIL)
			break;
		
		if (name[i]=='/')
		{

			if (count==level)
			{
				return false;
			}
			count++;
		}
		i++;
	}
	return true;

}
