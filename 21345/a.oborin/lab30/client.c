#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *socket_path = "./socket";

int main(int argc, char *argv[]) {

	struct sockaddr_un socket_address;

	int file_descriptor;
	ssize_t rc;

	file_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
	if (file_descriptor == -1) {
		perror("client: failed to create socket\n");
		return -1;
	}

	memset(&socket_address, 0, sizeof(socket_address));
	socket_address.sun_family = AF_UNIX;
	strncpy(socket_address.sun_path, socket_path, sizeof(socket_address.sun_path) - 1);

	if (connect(file_descriptor, (struct sockaddr*)&socket_address, sizeof(socket_address)) == -1) {
		perror("connect error");
		return -1;
	}

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	while ( (rc = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		if (write(file_descriptor, buffer, rc) != rc) {
			if (rc > 0) {
				fprintf(stderr, "partial write");
			} else {
				perror("write error");
				return -1;
			}
		}
	}
	return 0;
}
