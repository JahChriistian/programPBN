CC=gcc
CFLAGS=
RM=rm -rf
OUT=simulation

all: build

build: main.o states.o input.o graph.o
	$(CC) -lm $(CFLAGS) -o $(OUT) main.c input.c states.c graph.c -lm
	$(RM) *.o

debug: CFLAGS += -DDEBUG_ON
debug: build

main.o: main.c input.h states.h graph.h
	$(CC) $(CFLAGS) -c main.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

states.o: states.c states.h input.h
	$(CC) $(CFLAGS) -c states.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c
clean:
	$(RM) *.o $(OUT)


