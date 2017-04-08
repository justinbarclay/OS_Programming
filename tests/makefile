CC=gcc
CFLAGS = -std=c99

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

fileIO.o: ../fileIO.c ../fileIO.h 
	$(CC) $(CFLAGS) -c ../fileIO.c 
IOTest: fileIO.o fileIOTest.c
	$(CC) $(CFLAGS) -o IOTest fileIOTest.c fileIO.o


test1: listTest
	./listTest >> test_results.txt
test2: fifoTest
	./fifoTest >> test_results.txt
test3: lruTest
	./lruTest >> test_results.txt
test4: IOTest 
	./IOTest quicksort-trace.bin > IO_test_Results.txt
	less IO_test_Results.txt

runTests: test1 test2 test3
clean:
	rm *.o listTest *.txt lruTest fifoTest simul IOTest 
