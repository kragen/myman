all: copy

# Compiler options
#CCOPTS = -g -O3 -Wall -Werror
CCOPTS =

# Compiler
CC = gcc -Wall -Werror
#CC = cc

# Linker
LD = $(CC)

# Utility to remove a file
RM = rm

OBJS = main.o mygetopt.o

copy: $(OBJS)
	$(LD) -o $@ $(OBJS)

clean:
	$(RM) -f copy $(OBJS) *~

%.o: %.c getopt.h mygetopt.h
	$(CC) $(CCOPTS) -o $@ -c $<
