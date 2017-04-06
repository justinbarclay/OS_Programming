CC=gcc
#CFLAGS = -m32 -w -g

linkedlist.o: ../linkedlist.h ../linkedlist.c
	$(CC) $(CFLAGS) -c ../linkedlist.c
testList: linkedlist.o testList.c
	$(CC) $(CFLAGS) -o listTest listTest.c	
test1: listTest
	./listTest > test_results.txt
runTests: test1




clean:
	rm *.o testWhiteboard testHandleMessage parsingTest
