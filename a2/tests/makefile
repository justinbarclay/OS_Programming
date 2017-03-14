CC=gcc
IDIR=../
#CFLAGS = -m32 -w -g
p=makefilesarehorrid

parser.o: ../parser.c ../parser.h
	$(CC) $(CFLAGS) -c ../parser.c
testParser: parser.o
	$(CC) $(CFLAGS) -o parsingTest parsingTest.c parser.o

whiteboard.o: ../whiteboard.c ../whiteboard.h
	$(CC) $(CFLAGS) -c ../whiteboard.c -g

testWhiteboard: whiteboard.o testWhiteboard.c
	$(CC) $(CFLAGS) -o testWhiteboard testWhiteboard.c whiteboard.o -g

testHandleMessage: whiteboard.o parser.o testHandleMessage.c
	$(CC) $(CFLAGS) -o testHandleMessage testHandleMessage.c whiteboard.o parser.o -g

test1: testParser
	./parsingTest > test_results.txt

test2: testWhiteboard
	./testWhiteboard

test3: testHandleMessage
	./testHandleMessage

clean:
	rm *.o testWhiteboard testHandeMessage parsingTest
