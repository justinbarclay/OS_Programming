CC = gcc
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c
memory.o: linkedlist.o memory.c memory.h
	$(CC) -c memory.c linkedlist.o
simulation: simul.c linkedlist.o memory.o
	$(CC) -o simul simul.c linkedlist.o memory.o 

simulator: tvm379.c 
	gcc -o tvm379 tvm379.c 

clean:
	rm *.o tvm379
