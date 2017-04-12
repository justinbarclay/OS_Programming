CC = gcc
simulator: tvm379.c fileIO.o memory.o linkedlist.o
	$(CC) $(CFLAGS) -o tvm379 tvm379.c fileIO.o memory.o linkedlist.o
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c

memory.o: linkedlist.o memory.c memory.h
	$(CC) -c memory.c

fileIO.o: fileIO.c fileIO.h
	$(CC) $(CFLAGS) -c fileIO.c
clean:
	rm *.o tvm379
