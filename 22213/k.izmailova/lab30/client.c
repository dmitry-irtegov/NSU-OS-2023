#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int sfd;
    //создание сокета
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char* socketPath = "./socket";
    strcpy(addr.sun_path, socketPath);

    //соединение сокета и сервера
    if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(1);
    }

    int rc;
    char buf[BUFSIZ];
    while ((rc = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
        if (write(sfd, buf, rc) != rc) {
            if (rc > 0) {
                perror("partial write");
            }
            else {
                perror("write error");
                exit(1);
            }

        }
    }

    close(sfd);
    return 0;
}
