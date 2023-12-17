all: server client

server: server.c
	gcc -Wall -Werror -o server server.c -lsocket

client: client.c
	gcc -Wall -Werror -o client client.c -lsocket

clean:
	rm -f server client
