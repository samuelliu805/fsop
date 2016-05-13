#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_OPS 10		//maximum operations you want to record associated to a system call
#define MAX_PROCESS 10	//maximum process running at the same time 
#define MAX_DETAIL 50	//maximum detail regarding to a operation you want to record
#define TIME_OFFSET 14	//8 character offset at the beginning of each line in the to be pased file
#define	SPACE_OFFSET 6
#define MAX_LINE_SCAN 300
#define MAX_TERMINAL_NUM 50
#define MAX_DETAIL_FILES 50
#define	MAX_FILES_DETAIL 50

typedef struct file_ops{
	char operations[MAX_OPS][10];
	char op_detail[MAX_OPS][MAX_DETAIL];
	char time[MAX_OPS][TIME_OFFSET];
	int num_bytes[MAX_OPS];
	int syscall_num;
	int count;
	int thread_id;
	bool done;
	bool start;
}file_ops;

typedef struct terminal_ops{
	char content[MAX_DETAIL];
	bool start;

}terminal_ops;

typedef struct file_details{
	char filename[MAX_FILES_DETAIL];
	bool start;
	int write_freq;				//better to change to string in later stage 
	float write_time;
	int num_bytes_read;
	int num_bytes_write;
	int read_freq;
	float read_time;
	int operation;

}file_details;


void parse_files(char* filename);
int isvalidoperation(char* line,file_ops* history);
int getsyscall_num(char* line);
void getops_detail(char* buffer, char* line);
void printresults(file_ops* history, int ops);
void clearhistory(file_ops* history, int ops);
void printtoterminal();
void analyzeresult();
void analyzefile(file_ops* history, int ops);
void clearterminaldetail();
int get_bytes(char* line);


void visualize();
void recursive(file_details* current, int level, int start, int root);
void printlevel(char* string, int level);
bool issamedir(char* name, char* target, int level);
void getdirname(char* name, char* ret, int level);
bool isenddir(char* name, int level);

volatile int enable_library;
volatile FILE* fptr;
terminal_ops terminal[MAX_TERMINAL_NUM];
file_details detail[MAX_DETAIL_FILES];

int main ()
{
	//FILE* fptr;
	char filename[20];
	while(1)
	{
		printf("please input the file name you wish to parse\n ");
		scanf("%s",filename);
		fptr=fopen(filename,"r");
		if (fptr)
			break;
		printf("invalid file name\n");

	}

	enable_library=-1;
	while (1)
	{
		printf("Do you wish to include library files? Enter 1 for yes and 0 for no\n");
		scanf("%d",&enable_library);
		if (enable_library==0||enable_library==1)
			break;
		printf("Invalid input\n");
	}
	parse_files(filename);
	printtoterminal();
	analyzeresult();
	visualize();
}

void printtoterminal()
{
	printf("\n------------File Operation To Terminal------------\n");
	int i;
	for (i=0; i<MAX_TERMINAL_NUM; i++)
		if (terminal[i].start==true)
			printf("%s\n", terminal[i].content);
}

void analyzeresult()
{
	printf("\n------------Analyzing Result------------\n");
	int i;
	for (i=0; i<MAX_DETAIL_FILES; i++)
		if (detail[i].start==true)
		{
			if (detail[i].write_freq!=0||detail[i].read_freq!=0)
				printf("\nFor %s\n",detail[i].filename);
			if (detail[i].write_freq!=0)
				printf("Has %d write, bytes %d, used total %f seconds\n",detail[i].write_freq,detail[i].num_bytes_write, detail[i].write_time);
			if (detail[i].read_freq!=0)
				printf("Has %d read, bytes %d, used total %f seconds\n",detail[i].read_freq, detail[i].num_bytes_read,detail[i].read_time);
		}

	//printf("------------Suggestions------------\n");
}

void visualize()
{
	printf("\n------------Visualized Output------------\n");
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


	char toterminal[MAX_FILES_DETAIL];
	/*
	char test[50];
	memset(test,'\0',50);
	memcpy(test,"/lib/x86_64-linux-gnu/libc.so.6",40);
	//*test="/lib/x86_64-linux-gnu/libc.so.6";
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
			printf("%s\n",detail[i].filename);
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
		current->start==false;
		return;
	}
	if (root==1)
	{
		printlevel(toterminal,level);
	}
	int i;
	int curr;
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

void parse_files(char* filename)
{
	FILE *fptr;
	fptr=fopen(filename,"r");
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
			if (isvalidoperation(line, history)==1)
			{
				//printf("valid\n");
			}

		}
		//when we reach the end of file, if there are files that are still not closed, we will still print them out
		for (i=0; i<MAX_PROCESS; i++)
		{
			if (history[i].start==true)
			{
				printresults(history,i);
			}
		}

	}

}

int isvalidoperation(char* line,file_ops* history)
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
				printresults(history, i);
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

void printresults(file_ops* history, int ops)
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
		printf("Operation %d: %s, detail: %s, bytes: %d, time: %s\n", i, history[ops].operations[i],history[ops].op_detail[i],history[ops].num_bytes[i],history[ops].time[i]);
		//printf("sys_num: %c\n", history[ops].syscall_num);

	}
	printf("Operation %d: close\n\n",i);
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

void analyzefile(file_ops* history, int ops)
{
	int i,j,k;
	for (i=0; i<MAX_DETAIL_FILES; i++)
	{
		if ((strncmp(detail[i].filename,history[ops].op_detail[0],MAX_FILES_DETAIL)==0&&detail[i].start==true)||detail[i].start==false)
		{
			for (j=0; j<MAX_OPS; j++)
			{
				if (strncmp(history[ops].operations[j],"write",5)==0)
				{
					detail[i].write_freq++;
					detail[i].write_time=detail[i].write_time+atof(history[ops].time[j]);
					detail[i].num_bytes_write=detail[i].num_bytes_write+history[ops].num_bytes[j];
				}
				if (strncmp(history[ops].operations[j],"read",4)==0)
				{
					detail[i].read_freq++;
					//printf("%s\n %f\n\n",history[ops].time[j],atof(history[ops].time[j]));
					detail[i].read_time=detail[i].read_time+atof(history[ops].time[j]);
					detail[i].num_bytes_read=detail[i].num_bytes_read+history[ops].num_bytes[j];

				}
			}
			if (detail[i].start==false)
			{
				detail[i].start=true;
				memcpy(detail[i].filename, history[ops].op_detail[0], MAX_FILES_DETAIL);
			}
			return;
		}

	}
	printf("exceeded maximum file details recording");
}

void clearterminaldetail()
{
	int i;
	for (i=0; i<MAX_TERMINAL_NUM; i++)
	{
		memset(terminal[i].content,MAX_DETAIL,'\0');
		terminal[i].start=false;
	}
	for (i=0; i<MAX_DETAIL_FILES;i++)
	{
		detail[i].start=false;
		detail[i].write_freq=0;
		detail[i].write_time=0;
		detail[i].num_bytes_read=0;
		detail[i].num_bytes_write=0;
		detail[i].read_freq=0;
		detail[i].read_time=0;
		detail[i].operation=0;
		memset(detail[i].filename,MAX_FILES_DETAIL,'\0');
	}
}

/*
typedef struct file_details{
	char filename[MAX_FILES_DETAIL];
	bool start;
	int write_freq;				//better to change to string in later stage 
	int write_time;
	int read_freq;
	int read_time;
	int operation;

}file_details;*/