CC = gcc
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c

simulation: simul.c linkedlist.o
	$(CC) -o simul simul.c linkedlist.o

simulator: tvm379.c 
	gcc -o tvm379 tvm379.c 

clean:
	rm *.o tvm379
