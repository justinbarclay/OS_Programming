SUBDIRS:=$b(wildcard */.)
p=hihello
all: server client

parser.o: parser.h parser.c
	gcc -c parser.c parser.h

encryption.o: encryption.c encryption.h
	gcc -c encryption.c encryption.h -lcrypto -g

encryptionmac.o: encryption.c encryption.h
	gcc -c encryption.c encryption.h -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include

whiteboard.o: whiteboard.h whiteboard.c
	gcc -c whiteboard.c whiteboard.h -g

server: server.c parser.o whiteboard.o
	gcc -o wbs379 server.c parser.o whiteboard.o -lpthread

client: client.c parser.o encryption.o
	gcc -o wbc379 client.c parser.o encryption.o -lcrypto

clean:
	rm *.o wbc379 wbs379
