#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parse.h"
#include "analyze.h"
#include "parsefile.h"
#include "visualize.h"



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
