all: server client

server: server.c
	gcc -Wall -Werror -o server server.c -lrt -lsocket

client: client.c
	gcc -Wall -Werror -o client client.c -lrt -lsocket

clean:
	rm -f server client
