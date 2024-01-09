#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <ctype.h>

#define SERVER_SOCK "socket.sock"
#define BUF_SIZE 100


//читает
int main(int argc, char *argv[]) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("SOCK CREATE FAILURE");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_SOCK, sizeof(addr.sun_path)-1);

    unlink(SERVER_SOCK);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("BINDING FAILURE");
        return -1;
    }

    if (listen(fd, 1) == -1) {
        perror("LISTENING FAILURE");
        return -1;
    }

    int accepted = accept(fd, NULL, NULL);
    if (accepted == -1) {
        perror("ACCEPT FAILED");
        close(fd);
        return -1;
    }

    char buf[BUF_SIZE];
    int rc = read(accepted, buf, BUF_SIZE);
    while (rc > 0) {
        for(int i = 0; i < rc; i++){
            putchar(toupper(buf[i]));
        }
        rc = read(accepted, buf, BUF_SIZE);
    }
    if (rc == -1) {
        perror("Reading error!\n");
        return -1;
    }
    if (rc == 0) {
        close(accepted);
        close(fd);
        return 0;
    }
}