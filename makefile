CC = gcc
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c

memory.o: linkedlist.o memory.c memory.h
	$(CC) -c memory.c linkedlist.o

fileIO.o: fileIO.c fileIO.h
	$(CC) $(CFLAGS) -c fileIO.c

simulator: tvm379.c fileIO.o memory.o linkedlist.o
	$(CC) $(CFLAGS) -o tvm379 tvm379.c fileIO.o memory.o linkedlist.o

clean:
	rm *.o tvm379
