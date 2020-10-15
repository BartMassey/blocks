CC=clang
CFLAGS=-O3 -Wall
CLANG_ANALYZE = clang --analyzer-output plist-multi-file --analyze

SRCS =	astar.c blocks.c idastar.c infra.c misc.c \
	rastar.c ridastar.c score.c state.c \
	statepq.c stateht.c debug.c astargen.c idastargen.c \
	checksoln.c

OBJS =	astar.o blocks.o idastar.o infra.o misc.o \
	rastar.o ridastar.o score.o state.o \
	statepq.o stateht.o debug.o astargen.o idastargen.o \
	checksoln.o

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
	-/bin/rm -f $(OBJS) blocks picture.o picture *.plist

install: blocks picture
	/bin/cp blocks picture ../bin

analyze:
	$(CLANG_ANALYZE) $(SRCS)
