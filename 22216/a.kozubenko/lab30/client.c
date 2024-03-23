#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int clt_sock;
    struct sockaddr_un srv_sockaddr;
    struct sockaddr_un clt_sockaddr;

    char *dsock_file = "./dsock_file";

    clt_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clt_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un));
    srv_sockaddr.sun_family = AF_UNIX;
    strcpy(srv_sockaddr.sun_path, dsock_file);

    if (connect(clt_sock, (struct sockaddr*) &srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
        perror("connect");
        close(clt_sock);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    while ((bytes_read = read(0, buffer, BUFSIZ)) > 0) {
        if (write(clt_sock, buffer, bytes_read) == -1) {
            perror("write");
            close(clt_sock);
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0) {
            break;
        } else if (bytes_read == -1) {
            perror("read");
            close(clt_sock);
            exit(EXIT_FAILURE);
        }
    }

    close(clt_sock);
    exit(EXIT_SUCCESS);
}

