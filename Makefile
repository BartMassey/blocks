CC=gcc
CFLAGS=-g -Wall -Wno-implicit
#CFLAGS=-O4

OBJS = astar.o blocks.o idastar.o infra.o misc.o ridastar.o state.o statepq.o stateht.o

all: blocks picture

blocks: $(OBJS)
	$(CC) $(CFLAGS) -o blocks $(OBJS)

$(OBJS): blocks.h

idastar.o ridastar.o: idamisc.c

picture: picture.o
	$(CC) $(CFLAGS) -o picture picture.o

clean:
	-/bin/rm -f $(OBJS) blocks

realclean: clean
	-/bin/rm -f picture.o picture

install: blocks picture
	/bin/cp blocks picture ../bin
