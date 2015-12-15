CC = gcc
CFLAGS =
OPT = -O0
INCLUDES =
LFLAGS =
LIBS = -lm
DEBUG = -ggdb
SRCS = CACaves.c test.c
OBJS = $(SRCS:.c=.o) 
MAIN = CACaves_test

.PHONY: clean

all:    $(MAIN)
	@echo  CACaves compiled!

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) 

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUG) $(OPT) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

# DO NOT DELETE THIS LINE -- make depend needs it
