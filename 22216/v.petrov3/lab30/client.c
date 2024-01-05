#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#define PARTSIZE 256

char *PATHTOSOCKET = "./dsock";

int main() {
    int clt_sock;
    struct sockaddr_un clt_sockaddr;

    clt_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clt_sock == - 1) {
        perror("socket failed");
        exit(1);
    }

    memset(&clt_sockaddr, 0, sizeof(struct sockaddr_un));
    clt_sockaddr.sun_family = AF_UNIX;
    strcpy(clt_sockaddr.sun_path, PATHTOSOCKET);

    if (connect(clt_sock,(const struct sockaddr*)&clt_sockaddr, sizeof(clt_sockaddr)) == -1) {
        perror("connect failed");
        close(clt_sock);
        exit(1);
    }
    
    int len = 0;
    char message_part[PARTSIZE] = {0};
    while(1) {
        len = read(0, message_part, PARTSIZE);
        if (len == -1) {
            perror("read failed");
            close(clt_sock);
            exit(1);
        }
        write(clt_sock, message_part, len);
    }

    close(clt_sock);
    return 0;
}
