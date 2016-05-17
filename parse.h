#ifndef _PARSE_H
#define _PARSE_H

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


volatile int enable_library;
volatile FILE* fptr;
terminal_ops terminal[MAX_TERMINAL_NUM];
file_details detail[MAX_DETAIL_FILES];

#endif
