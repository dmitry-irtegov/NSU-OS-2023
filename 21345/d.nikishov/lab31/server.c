#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>

#define MAX_CLIENTS 10

void unlinker(void) {
    unlink("./socket");
}

void handler() {
    unlink("./socket");
    _exit(0);
}

int main() {
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Error while creating socket");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char* socket_path = "./socket";
    strcpy(addr.sun_path, socket_path);

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error while binding");
        close(socket_fd);
        return 1;
    }

    atexit(unlinker);
    signal(SIGINT, handler);

    if (listen(socket_fd, MAX_CLIENTS) == -1) {
        perror("Error while listening");
        close(socket_fd);
        return 1;
    }

    fd_set master_fds;
    FD_ZERO(&master_fds);
    FD_SET(socket_fd, &master_fds);

    int max_fd = socket_fd;
    char* message;

    while (1) {
        fd_set read_fds = master_fds;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error in select");
            break;
        }

        for (int i = 0; i <= max_fd; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == socket_fd) {
                    int connection_fd = accept(socket_fd, NULL, NULL);
                    if (connection_fd == -1) {
                        perror("Error while accepting");
                    }
                    else {
                        message = "New connection established.\n";
                        write(STDOUT_FILENO, message, strlen(message));
                        FD_SET(connection_fd, &master_fds);
                        if (connection_fd > max_fd) {
                            max_fd = connection_fd;
                        }
                    }
                }
                else {
                    char buf[BUFSIZ];
                    ssize_t read_count = read(i, buf, BUFSIZ);
                    if (read_count > 0) {
                        for (int j = 0; j < read_count; ++j) {
                            buf[j] = toupper(buf[j]);
                        }
                        if (write(STDOUT_FILENO, buf, read_count) != read_count) {
                            perror("Error while writing");
                        }
                    }
                    else if (read_count == 0) {
                        message = "Connection closed.\n";
                        write(STDOUT_FILENO, message, strlen(message));
                        close(i);
                        FD_CLR(i, &master_fds);
                    }
                    else {
                        perror("Error while reading");
                        close(i);
                        FD_CLR(i, &master_fds);
                    }
                }
            }
        }
    }

    close(socket_fd);
    return 0;
}
