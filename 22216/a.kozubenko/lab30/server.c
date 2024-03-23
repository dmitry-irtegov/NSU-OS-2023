#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int srv_sock;
    int clt_sock;
    struct sockaddr_un srv_sockaddr;
    struct sockaddr_un clt_sockaddr;

    char *dsock_file = "./dsock_file";

    srv_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (srv_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un));
    srv_sockaddr.sun_family = AF_UNIX;
    strcpy(srv_sockaddr.sun_path, dsock_file);

    unlink(dsock_file);

    if (bind(srv_sock, (struct sockaddr *) &srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
        perror("bind");
        close(srv_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(srv_sock, 0) == -1) {
        perror("listen");
        close(srv_sock);
        exit(EXIT_FAILURE);
    }

    memset(&clt_sockaddr, 0, sizeof(struct sockaddr_un));
    clt_sock = accept(srv_sock, NULL, NULL);
    if (clt_sock == -1) {
        perror("accept");
        close(srv_sock);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    while (1) {
        while ((bytes_read = read(clt_sock, buffer, BUFSIZ)) > 0) {
            for (int i = 0; i < bytes_read; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            if (write(1, buffer, bytes_read) == -1) {
                perror("write");
                close(srv_sock);
                close(clt_sock);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == 0) {
            break;
        } else if (bytes_read == -1) {
            perror("read");
            close(srv_sock);
            close(clt_sock);
            exit(EXIT_FAILURE);
        }
    }

    close(srv_sock);
    close(clt_sock);

    exit(EXIT_SUCCESS);
}

