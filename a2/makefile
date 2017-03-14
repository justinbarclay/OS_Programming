SUBDIRS:=$b(wildcard */.)
p=hihello
parser.o: parser.h parser.c
	gcc -c parser.c parser.h

server: server.c parser.o
	gcc -o server server.c parser.o

client: client.c
	gcc -o client client.c -lncurses -lpthread


tests:$(SUBDIRS)
	export
	cd tests && make all

.PHONY: tests $(SUBDIRS)
