#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>


#include "analyze.h"
#include "parse.h"
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



void analyzefile(file_ops* history, int ops)
{
	int i,j;
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
