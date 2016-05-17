#ifndef _VISUALIZE_H
#define _VISUALIZE_H
#include "parse.h"



void visualize();
void recursive(file_details* current, int level, int start, int root);
void printlevel(char* string, int level);
bool issamedir(char* name, char* target, int level);
void getdirname(char* name, char* ret, int level);
bool isenddir(char* name, int level);

#endif