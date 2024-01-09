all: server client
.PHONY: all
server: server.c
	gcc -o server server.c -lsocket

client: client.c
	gcc -o client client.c -lsocket