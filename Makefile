CC=gcc
# CFLAGS=-g -Wall -Wno-implicit
CFLAGS=-O4

OBJS = astar.o blocks.o idastar.o infra.o misc.o state.o

all: blocks picture

blocks: $(OBJS)
	$(CC) $(CFLAGS) -o blocks $(OBJS)

$(OBJS): blocks.h

picture: picture.o
	$(CC) $(CFLAGS) -o picture picture.o

clean:
	-rm -f $(OBJS) picture.o blocks picture
