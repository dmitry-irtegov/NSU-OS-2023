#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>

#define PARTSIZE 256

char *PATHTOSOCKET = "./dsock";

int write_part(char *message_part, int len) {
    for (int i = 0; i < len; i ++) {
        message_part[i] = toupper(message_part[i]);
    }

    if (write(1, message_part, len) == -1) {
        return -1;
    } 
    return 0;
}

int main() {
    int srv_sock;
    int clt_sock;
    int err;
    struct sockaddr_un srv_sockaddr;
    struct sockaddr_un clt_sockaddr;
    
    signal(SIGPIPE, SIG_IGN);
    srv_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (srv_sock == - 1) {
        perror("socket failed");
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un));
    srv_sockaddr.sun_family = AF_UNIX;
    strcpy(srv_sockaddr.sun_path, PATHTOSOCKET);

    err = bind(srv_sock, (const struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr));
    if (err == -1) {
        perror("bind failed");
        close(srv_sock);
        exit(1);
    }

    err = listen(srv_sock, 1);
    if (err == -1) {
        perror("listen failed");
        close(srv_sock);
        exit(1);
    }

    clt_sock = accept(srv_sock, NULL, NULL);
    if (clt_sock == -1) {
        perror("accept failed");
        close(srv_sock);
        exit(1);
    }

    char message_part[PARTSIZE] = {0};
    int len = 0;
    while ((len = read(clt_sock, message_part, PARTSIZE)) > 0) {
        if (write_part(message_part, len) == -1) {
            perror("write failed");
            close(srv_sock);
            close(clt_sock);
            exit(1);
        }
    }

    if (len == -1) {
        perror("read failed");
        close(srv_sock); close(clt_sock);
        exit(1);
    }

    close(srv_sock);
    close(clt_sock);
    return 0;
}
