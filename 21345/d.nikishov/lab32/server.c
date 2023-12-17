#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>

void unlinker(void)
{
        unlink("./socket");
}

void handler()
{
        unlink("./socket");
        _exit(0);
}

int main() {

    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Error while create socket");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char* socket_path = "./socket";
    strcpy(addr.sun_path, socket_path);

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error while bind");
        close(socket_fd);
        close(socket_fd);
        return 1;
    }

    atexit(unlinker);
    signal(SIGINT,handler);
    if (listen(socket_fd, 1) == -1) {
        perror("Error while listen");
        close(socket_fd);
        return 1;
    }


    int connection_fd = accept(socket_fd, NULL, NULL);
    if (connection_fd == -1) {
        perror("Error while accepting");
        close(socket_fd);
        close(connection_fd);
        return 1;
    }

    printf("Connected:\n");
    ssize_t read_count;
    char buf[BUFSIZ];

    while ((read_count = read(connection_fd, buf, BUFSIZ)) > 0) {
        for (int i = 0; i < read_count; ++i) {
            buf[i] = toupper(buf[i]);
        }
        if (write(STDOUT_FILENO, buf, read_count) != read_count) {
            perror("Error while writing");
            close(socket_fd);
            close(connection_fd);
            return 1;
        }
    }

    if (read_count == -1) {
        perror("Error while read");
        close(socket_fd);
        close(connection_fd);
        return 1;
    }
    if (read_count == 0) {
        printf("Connection closed\n");
    }

    close(socket_fd);
    close(connection_fd);
    return 0;
}
