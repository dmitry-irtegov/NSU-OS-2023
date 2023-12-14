#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <netinet/in.h>

#define SERVER_SOCK_FILE "server.sock"
#define MSGSIZE 20

int file_descriptor = -1;

void pipe_sig_handler() {
    if (file_descriptor != -1) {
        close(file_descriptor);
        write(2, "Writing to the socket failure\n", 31);
    }

    exit(EXIT_FAILURE);
}

void int_sig_handler() {
    if (file_descriptor != -1) {
        close(file_descriptor);
    }
    write(1, "\nThat's all\n", 22);
    
    exit(EXIT_SUCCESS);
}

int main() {
	struct sockaddr_un sock_addr;
	char msgout[MSGSIZE];
    signal(SIGPIPE, pipe_sig_handler);
    signal(SIGINT, int_sig_handler);

    if ((file_descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Cannot create socket");
		exit(EXIT_FAILURE);
	}

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, SERVER_SOCK_FILE);
    if (connect(file_descriptor, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("Connection failure");
        exit(EXIT_FAILURE);
    }

    int read_size = 0;
    while (1) {
        read_size = read(0, msgout, MSGSIZE);
        size_t to_send = read_size < MSGSIZE ? read_size : MSGSIZE;
        write(file_descriptor, msgout, to_send);
    };
    
    close(file_descriptor);
	exit(EXIT_SUCCESS);
}
