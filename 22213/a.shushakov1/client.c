#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SOCK_FILE "server.sock"

int descriptor = -1;

void sigPipeHandler();

int main() {
    struct sockaddr_un addr;
    signal(SIGPIPE, sigPipeHandler);

    if ((descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_FILE, sizeof(addr.sun_path) - 1);

    if (connect(descriptor, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("Connection failure");
        close(descriptor);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server\n");

    char msgout[BUFSIZ];

    while (1) {
        long red;
        if ((red = read(STDIN_FILENO, msgout, BUFSIZ)) == -1) {
            perror("Read from buffer error!");
            close(descriptor);
            exit(EXIT_FAILURE);
        }

        size_t dataLength = red < BUFSIZ ? red : BUFSIZ;

        if ((write(descriptor, msgout, dataLength)) == -1) {
            perror("Write to buffer error!");
            close(descriptor);
            exit(EXIT_FAILURE);
        }
    }
    close(descriptor);
	exit(EXIT_SUCCESS);
}

void sigPipeHandler() {
    if (descriptor != -1) {
        close(descriptor);
        write(STDERR_FILENO, "Error when writing to the socket!\n", 35);
    }
    _exit(EXIT_FAILURE);
}
