EXENAME = fsop
DEPENDENCIES = parse.c parsefile.c analyze.c visualize.c
OBJS = parse.o parsefile.o analyze.o visualize.o
CC = gcc
FLAGS = -Wall -Wextra -g -std=c99

LD = gcc
LDFLAGS = -std=c99 -g -Wall -Wextra 

all	: $(EXENAME)
.PHONY: all

$(EXENAME) : $(OBJS)
	$(CC) $^ -o $@

parse.o : parse.c parse.h parsefile.h analyze.h visualize.h
	$(CC) -c $(FLAGS) $< -o $@

parsefile.o : parsefile.c parsefile.h parse.h
	$(CC) -c $(FLAGS) $< -o $@

analyze.o : analyze.c analyze.h parse.h
	$(CC) -c $(FLAGS) $< -o $@

visualize.o : visualize.c visualize.h parse.h
	$(CC) -c $(FLAGS) $< -o $@

clean : 
	-rm -f *.o $(EXENAME)
