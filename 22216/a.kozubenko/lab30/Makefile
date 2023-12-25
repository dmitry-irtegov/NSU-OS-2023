all: server client
server: server.c
	cc -v server.c -lsocket -lnsl -o server
client: client.c
	cc -v client.c -lsocket -lnsl -o client
