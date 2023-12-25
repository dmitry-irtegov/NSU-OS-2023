#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

char *socket_path = "server_socket";

void sighandler(int signum) {
    fprintf(stdout, "server is closed\n");
    exit(EXIT_SUCCESS);
}

int main() {
    sigset(SIGPIPE, sighandler);

    int sock_fd;
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path);

    if (connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    while ((bytes_read = read(0, buffer, BUFSIZ)) > 0) {
        write(sock_fd, buffer, bytes_read);
    }

    exit(EXIT_SUCCESS);
}

