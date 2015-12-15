CC = gcc
CFLAGS =
OPT = -O0
INCLUDES =
LFLAGS =
LIBS = -lm
DEBUG = -ggdb
SRCS = teleport.c level.c player.c globals.c object.c caves.c monster.c game.c msgBox.c utils.c KeyPress/KeyPress.c CACaves/CACaves.c
OBJS = $(SRCS:.c=.o) 
MAIN = rogue

.PHONY: clean

all:    $(MAIN)
	@echo  Rogue compiled!

$(MAIN): $(OBJS)
	cd CACaves; make
	cd KeyPress; make
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) 

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUG) $(OPT) -c $<  -o $@

clean:
	cd CACaves; make clean
	cd KeyPress; make clean
	$(RM) *.o *~ $(MAIN)

# DO NOT DELETE THIS LINE -- make depend needs it
