#ifndef _PARSEFILE_H
#define _PARSEFILE_H
#include "parse.h"
#include "analyze.h"

void parse_files(char* filename, char * outputFile);
int isvalidoperation(FILE * outfp, char* line,file_ops* history);
int getsyscall_num(char* line);
void getops_detail(char* buffer, char* line);
void printresults(FILE *fp, file_ops* history, int ops);
int get_bytes(char* line);
void clearhistory(file_ops* history, int ops);

#endif 
