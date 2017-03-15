SUBDIRS:=$b(wildcard */.)
p=hihello
all: server client

parser.o: parser.h parser.c
	gcc -c parser.c parser.h

whiteboard.o: whiteboard.h whiteboard.c
	gcc -c whiteboard.c whiteboard.h

server: server.c parser.o whiteboard.o
	gcc -o wbs379 server.c parser.o whiteboard.o -lpthread

client: client.c parser.o
	gcc -o wbc379 client.c parser.o

clean:
	rm *.o wbc379 wbs379

tests:$(SUBDIRS)
	export
	cd tests && make all

.PHONY: tests $(SUBDIRS)
