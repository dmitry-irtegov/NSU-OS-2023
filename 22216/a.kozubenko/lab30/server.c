#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

char *socket_path = "server_socket";
int socket_fd;

void close_unlink() {
    if (close(socket_fd) == -1) {
        perror("close");
    }
    if (unlink(socket_path) == -1) {
        perror("unlink");
    }
}

int main() {
    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    atexit(close_unlink);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path);

    if (bind(socket_fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, 0) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int client_fd;
    if ((client_fd = accept(socket_fd, NULL, NULL)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    while (1) {
        while ((bytes_read = read(client_fd, buffer, BUFSIZ)) > 0) {
            for (int i = 0; i < bytes_read; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            write(1, buffer, bytes_read);
        }

        if (bytes_read == 0) {
            close(client_fd);
            break;
        }
    }

    exit(EXIT_SUCCESS);
}

