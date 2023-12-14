#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <netinet/in.h>

#define SERVER_SOCK_FILE "server.sock"
#define MSGSIZE 20


int main() {
	int fd, length, status;
	struct sockaddr_un sock_addr;
	char got_message[MSGSIZE];
	memset(got_message, '\0', MSGSIZE);

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation failure");
		exit(EXIT_FAILURE);
	}

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, SERVER_SOCK_FILE);
	
	unlink(SERVER_SOCK_FILE);
	if ((status = bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr))) < 0) {
		perror("Binding failure");
		exit(EXIT_FAILURE);
	}
	if (listen(fd, 1) == -1) {
		perror("Listening failure");
	}
	
	int accepted;
	if ((accepted = accept(fd, NULL, NULL)) == -1) {
		perror("Accept failed");
	}
		
	while ((length = read(accepted, got_message, MSGSIZE)) > 0) {
		for (int i = 0; i < length; i++) {
			got_message[i] = toupper(got_message[i]);
		}
		if ((write(1, got_message, length)) == -1) {
			perror("Write failure");
			exit(EXIT_FAILURE);
		};
	};

	if (length == -1) {
		perror("Read failure");
		exit(EXIT_FAILURE);
	}

	close(accepted);
	close(fd);
	unlink(SERVER_SOCK_FILE);
	exit(EXIT_SUCCESS);
}
