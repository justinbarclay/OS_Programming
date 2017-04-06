CC=gcc
#CFLAGS = -m32 -w -g

linkedlist.o: ../linkedlist.h ../linkedlist.c
	$(CC) $(CFLAGS) -c ../linkedlist.c
listTest: linkedlist.o listTest.c
	$(CC) $(CFLAGS) -o listTest listTest.c linkedlist.o
test1: listTest
	./listTest > test_results.txt
runTests: test1

clean:
	rm *.o listTest
