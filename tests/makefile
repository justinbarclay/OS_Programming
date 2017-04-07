CC=gcc
CFLAGS = -std=c99

linkedlist.o: ../linkedlist.h ../linkedlist.c
	$(CC) $(CFLAGS) -c ../linkedlist.c
listTest: linkedlist.o listTest.c
	$(CC) $(CFLAGS) -o listTest listTest.c linkedlist.o
test1: listTest
	./listTest > test_results.txt
runTests: test1

fileIO.o: ../fileIO.c ../fileIO.h 
	$(CC) $(CFLAGS) -c ../fileIO.c 
IOTest: fileIO.o fileIOTest.c
	$(CC) $(CFLAGS) -o IOTest fileIOTest.c fileIO.o
test2: IOTest 
	./IOTest > IO_test_Results.txt
runTests: test1

clean:
	rm *.o listTest IOTest
