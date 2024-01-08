#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Incorrect usage. Use: %s <port> <file>", argv[0]);
        return -1;
    }

    int proxyPort = atoi(argv[1]);
    if (!proxyPort) {
        fprintf(stderr, "Incorrect port.\n");
        return -1;
    }

    int fdIn = 0;
    if (argc == 3) {
        if ((fdIn = open(argv[2], O_RDONLY)) == -1) {
            perror("Open failed");
            exit(-1);
        }
    }

    char buffer[BUFSIZ];
    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(proxyPort);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect failed");
        exit(-1);
    }

    size_t bytesRead;
    bytesRead = read(fdIn, buffer, BUFSIZ);

    write(socketFd, buffer, bytesRead);


    bytesRead = read(socketFd, buffer, BUFSIZ);
    write(1, buffer, bytesRead);

    close(socketFd);

    return 0;
}
