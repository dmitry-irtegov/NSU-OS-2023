#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SOCKET_PATH "./socket"


int main(int argc, char* argv[]) {
    int fdIn = 0;
    if (argc == 2) {
        if ((fdIn = open(argv[1], O_RDONLY)) == -1) {
            perror("Open faieled");
            exit(-1);
        }
    }


    char buffer[BUFSIZ];
    int socketFd;
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_un clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, SOCKET_PATH, sizeof(clientAddr.sun_path) - 1);

    if (connect(socketFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1) {
        perror("Connect failed");
        exit(-1);
    }

    size_t byteRead;
    while ((byteRead = read(fdIn, buffer, BUFSIZ)) > 0) {
        write(socketFd, buffer, byteRead);
    }

    return 0;
}
