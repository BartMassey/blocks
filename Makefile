CC=gcc
CFLAGS=-g -Wall -Wno-implicit
#CFLAGS=-O4

OBJS =	astar.o blocks.o idastar.o infra.o misc.o \
	rastar.o ridastar.o score.o state.o \
	statepq.o stateht.o debug.o astargen.o idastargen.o

all: blocks picture

blocks: $(OBJS)
	$(CC) $(CFLAGS) -o blocks $(OBJS)

$(OBJS): blocks.h

idastar.o ridastar.o idastargen.o: idaprotect.c

debug.o: writepicture.c

picture.o: writepicture.c

picture: picture.o
	$(CC) $(CFLAGS) -o picture picture.o

clean:
	-/bin/rm -f $(OBJS) blocks

realclean: clean
	-/bin/rm -f picture.o picture

install: blocks picture
	/bin/cp blocks picture ../bin

