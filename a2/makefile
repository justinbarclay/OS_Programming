SUBDIRS:=$b(wildcard */.)
p=hihello
parser.o: parser.h parser.c
	gcc -c parser.c parser.h

whiteboard.o: whiteboard.h whiteboard.c
	gcc -c whiteboard.c whiteboard.h

server: server.c parser.o whiteboard.o
	gcc -o server server.c parser.o whiteboard.o

client: client2.c parser.o
	gcc -o client client2.c parser.o -lncurses -lpthread


tests:$(SUBDIRS)
	export
	cd tests && make all

.PHONY: tests $(SUBDIRS)
