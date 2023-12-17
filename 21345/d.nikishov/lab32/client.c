#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>


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

    if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error while connecting");
        close(socket_fd);
        return 1;
    }

    int read_count;
    char buf[BUFSIZ];
    while ((read_count = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
        if (write(socket_fd, buf, read_count) != read_count) {
            perror("Error while writing");
            close(socket_fd);
            return 1;
        }
    }
    close(socket_fd);
    return 0;
}
