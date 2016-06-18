#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parse.h"
#include "analyze.h"
#include "parsefile.h"
#include "visualize.h"


int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Please include a filename and library file flag (1 for yes, 0 for no)!\n");
        return -1;
    }

    char *filename = argv[1];
    FILE * fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        fprintf(stderr, "Wrong filename\n");
        return -1;
    }
    
	enable_library = atoi(argv[2]);
    if (enable_library != 0 && enable_library != 1)
    {
        fprintf(stderr, "Wrong flag value!\n");
        return -1;
    }
    if (argc == 4)
    {
        //FILE * outfp = fopen(argv[3], "w");
        parse_files(filename, argv[3]);
    }
    else
    {   
 	    parse_files(filename, NULL);
    }
    printtoterminal();
	analyzeresult();
	visualize();
}
