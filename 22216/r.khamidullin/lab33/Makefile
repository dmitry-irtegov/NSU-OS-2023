.PHONY: clean

all: server.out client.out proxy.out
client.out:
	cc -v client.c -lnsl -lsocket -o client.out
server.out:
	cc -v server.c -lnsl -lsocket -o server.out
proxy.out:
	cc -v proxy.c -lnsl -lsocket -o proxy.out
clean:
	rm -f server.out
	rm -f client.out
	rm -f proxy.out
