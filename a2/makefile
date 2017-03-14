SUBDIRS:=$b(wildcard */.)
p=hihello
parser.o: parser.h parser.c
	gcc -c parser.c parser.h

encryption.o: encryption.c encryption.h
	gcc -c encryption.c encryption.h -lcrypto 

whiteboard.o: whiteboard.h whiteboard.c
	gcc -c whiteboard.c whiteboard.h

server: server.c parser.o whiteboard.o
	gcc -o server server.c parser.o whiteboard.o

client: client.c parser.o encryption.o
	gcc -o client client.c parser.o encryption.o -lcrypto

tests:$(SUBDIRS)
	export
	cd tests && make all

.PHONY: tests $(SUBDIRS)
