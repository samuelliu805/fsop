#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parsefile.h"
#include "parse.h"

void parse_files(char* filename, char * outfilename)
{
	FILE *fptr = fopen(filename,"r");
    FILE *outfp;
    if (outfilename != NULL) 
    {
        outfp = fopen(outfilename, "w");
    }
    else 
    {
        outfp = stdout;
    }

	if (fptr==NULL)
	{
		printf("cannot find file\n");
		return;
	}
	else
	{
		char line[MAX_LINE_SCAN];
		file_ops history[MAX_PROCESS];
		int i=0;
		for (i=0; i<MAX_PROCESS; i++)
		{
			clearhistory(history,i);
		}
		while (fgets(line, MAX_LINE_SCAN, fptr))	
		{
			//printf("line: %s\n",line);
			if (isvalidoperation(outfp, line, history)==1)
			{
				//printf("valid\n");
			}

		}
		//when we reach the end of file, if there are files that are still not closed, we will still print them out
		for (i=0; i<MAX_PROCESS; i++)
		{
			if (history[i].start==true)
			{
				printresults(outfp, history,i);
			}
		}

	}

}

int isvalidoperation(FILE * outfp, char* line,file_ops* history)
{
	if (!line||!line[0]||!line[1]||!line[2]||!line[3])
		return 0;
	char* cur_time=line;
	cur_time=cur_time+SPACE_OFFSET-1;
	line=line+TIME_OFFSET;
		//printf("%s",line);

	if (strncmp(line,"open",4)==0)
	{
		int i=0;
		while(history[i].start==true)
		{
			i++;
		}
		if (i==MAX_PROCESS)
		{
			printf("Exceeded the maximum supported process\n");
			return 1;
		}

		history[i].start=true;
		history[i].count=0;
		memcpy(history[i].time[0],cur_time,TIME_OFFSET-SPACE_OFFSET+1);
		memcpy(history[i].operations[0],"open",4);
		history[i].syscall_num=getsyscall_num(line);
		getops_detail(history[i].op_detail[0],line);
		//printf("history:%c, name %s\n",history[i].syscall_num,history[i].op_detail[0]);
		//if (history[0].syscall_num!=51)
		//	printf("&&&&&\n");
		return 1;
	}
	if (strncmp(line,"close",5)==0)
	{
		int sys_num=line[6];
		//printf("sys_num:%c",sys_num);
		int i=0;
		for (i=0; i<MAX_PROCESS; i++)
		{
			if (history[i].syscall_num==sys_num)
			{
				printresults(outfp, history, i);
				clearhistory(history, i);
				return 1;
			}
		}
		printf("Unable to find operations-file is not opened yet!\n");
		return 1;
	}
	
	if (strncmp(line,"read(",5)==0)
	{
		int sys_num=line[5];
		int i;
		//printf("in read, sys num: %d\n", sys_num);
		for (i=0; i<MAX_PROCESS; i++)
		{
			if (history[i].syscall_num==sys_num)
			{
				if (history[i].count<MAX_OPS-1)
					history[i].count++;

				memcpy(history[i].operations[history[i].count],"read",4);
				memcpy(history[i].time[history[i].count],cur_time,TIME_OFFSET-SPACE_OFFSET+1);
				getops_detail(history[i].op_detail[history[i].count],line);
				history[i].num_bytes[history[i].count]=get_bytes(line);
				return 1;
			}
		}
		printf("Unable to find operations-file is not opened yet!\n");
		return 1;
	}

	if(strncmp(line,"write",5)==0)
	{
		int sys_num=line[6];
		int i;
		for (i=0; i<MAX_PROCESS; i++)
		{
			if (history[i].syscall_num==sys_num)
			{
				if (history[i].count<MAX_OPS-1)
					history[i].count++;
				/*
				history[i].operations[history[i].count][0]='w';
				history[i].operations[history[i].count][1]='r';
				history[i].operations[history[i].count][2]='i';
				history[i].operations[history[i].count][3]='t';
				history[i].operations[history[i].count][3]='e';
				*/
				memcpy(history[i].operations[history[i].count],"write",5);
				memcpy(history[i].time[history[i].count],cur_time,TIME_OFFSET-SPACE_OFFSET+1);
				//printf("%s\n",history[i].time[history[i].count]);
				getops_detail(history[i].op_detail[history[i].count],line);
				history[i].num_bytes[history[i].count]=get_bytes(line);
				return 1;

			}
		}
		//printf("%d",sys_num);
		if (sys_num=='1')
		{
			for (i=0; i<MAX_TERMINAL_NUM;i++)
			{
				if (terminal[i].start==false)
				{
					getops_detail(terminal[i].content,line);
					terminal[i].start=true;
				}
			}
			return 1;

		}
		printf("Unable to find operations-file is not opened yet!\n");
		return 1;
	}

    return 1;
}


int getsyscall_num(char* line)
{
	int i=0; 
	while (1)
	{
		if (i==300)
		{
			printf("unable to get the system call number\n");
			return -1;
		}
		if (line[i]=='=')
		{
			if (isdigit(line[i+2]))
				break;
		}
		i++;
	}
		//printf("getting syscall num: %c",line[i+2]);

	return line[i+2];
}

int get_bytes(char* line)
{

	int i=0; 
	int j=0;
	//printf("in get bytes\n");
	char result[10];
	for (j=0; j<10; j++)
		result[j]='\0';
	j=0;
	while (1)
	{
		if (i==300)
		{
			printf("unable to get the system call number\n");
			return -1;
		}
		if (line[i]=='=')
		{
			while (isdigit(line[i+2]))
			{
				result[j]=line[i+2];
				j++;
				i++;
			}
			//printf("%s\n%d\n",line,atoi(result));
			return atoi(result);
		}
		i++;
	}
}

void getops_detail(char* buffer, char* line)
{
	int i=0;
	while (1)
	{
		
		if (i==300)
		{
			printf("unable to find the detail associated with this operation\n");
			return;
		}
		
		if (line[i]=='"')
		{
			i++;
			int count=0;
			while (1)
			{
				if (line[i]=='"')
					return;
				buffer[count]=line[i];
				count++;
				i++;
				if (count==MAX_DETAIL-1)
					return;

			}
		}
		i++;

	}
}

/*typedef struct file_ops{
	char operations[10][20];
	char op_detail[10][20];
	int syscall_num;
	int count;
	bool done;
	bool start;
}file_ops;*/
 

void printresults(FILE* fp, file_ops* history, int ops)
{
	//printf("in print result, %s\n",history[ops].op_detail[0]);
	if (enable_library==0)
	{
		if (strncmp(history[ops].op_detail[0],"/lib/",5)==0||strncmp(history[ops].op_detail[0],"/etc/",5)==0||strncmp(history[ops].op_detail[0],"/usr/lib/",9)==0)
			return;
	}
	int i=0;
	for (i=0; i<=history[ops].count;i++)
	{
		fprintf(fp,"Operation %d: %s, detail: %s, bytes: %d, time: %s\n", i, history[ops].operations[i],history[ops].op_detail[i],history[ops].num_bytes[i],history[ops].time[i]);
		//printf("sys_num: %c\n", history[ops].syscall_num);

	}
	fprintf(fp, "Operation %d: close\n\n",i);
}

void clearhistory(file_ops* history, int ops)
{
	int i,j;
	analyzefile(history,ops);
	history[ops].start=false;
	history[ops].done=false;
	history[ops].count=0;
	history[ops].syscall_num=-1;
	history[ops].thread_id=-1;
	for (i=0; i<MAX_OPS; i++)
	{
		for (j=0; j<MAX_DETAIL; j++)
		{
			history[ops].operations[i][j]='\0';
			history[ops].op_detail[i][j]='\0';
		}
		memset(history[ops].time[i],TIME_OFFSET,'\0');
		history[ops].num_bytes[i]=-1;
	}

}
