CC=gcc
#CFLAGS = -m32 -w -g

linkedlist.o: ../linkedlist.h ../linkedlist.c
	$(CC) $(CFLAGS) -c ../linkedlist.c

memory.o: ../memory.h ../memory.c linkedlist.o
	$(CC) $(CFLAGS) -c ../memory.c linkedlist.o

listTest: linkedlist.o listTest.c
	$(CC) $(CFLAGS) -o listTest listTest.c linkedlist.o

fifoTest: fifoTest.c linkedlist.o memory.o
	$(CC) -o fifoTest fifoTest.c linkedlist.o memory.o
lruTest: lruTest.c linkedlist.o memory.o
	$(CC) -o lruTest lruTest.c linkedlist.o memory.o

test1: listTest
	./listTest >> test_results.txt
test2: fifoTest
	./fifoTest >> test_results.txt
test3: lruTest
	./lruTest >> test_results.txt

runTests: test1 test2 test3

clean:
	rm *.o listTest *.txt lruTest fifoTest simul
