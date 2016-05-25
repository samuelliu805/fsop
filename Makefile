EXENAME = fsop
DEPENDENCIES = parse.c parsefile.c analyze.c visualize.c

LD = gcc
LDFLAGS = -std=c99 -g -Wall -Wextra 

all	: $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(DEPENDENCIES) $(LDFLAGS) -o $(EXENAME)  

clean : 
	-rm -f *.o $(EXENAME)
