CC=gcc
CFLAGS=-Wall -Wextra -Iinclude -O2

all: server client

server: src/server.c
	$(CC) $(CFLAGS) -o server src/server.c

client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c

clean:
	rm -f server client

run-server: server
	./server 5050

run-client: client
	./client 127.0.0.1 5050