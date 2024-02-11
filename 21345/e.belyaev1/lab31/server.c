#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>

#define SOCKET_PATH "./socket"
#define MAX_CLIENTS 3

void sigCatch(int sig) {
    unlink(SOCKET_PATH);
    exit(0);
}

int main() {
    char buffer[BUFSIZ];
    int socketFd;
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SOCKET_PATH, sizeof(serverAddr.sun_path) - 1);

    if (bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(-1);
    }
    signal(SIGINT, sigCatch);
    signal(SIGQUIT, sigCatch);

    if (listen(socketFd, MAX_CLIENTS) == -1) {
        unlink(SOCKET_PATH);
        perror("Listen error");
        exit(-1);
    }

    struct pollfd pollFds[MAX_CLIENTS + 1];
    memset(pollFds, 0, sizeof(pollFds));

    pollFds[0].fd = socketFd;
    pollFds[0].events = POLLIN;

    nfds_t clientCnt = 0;

    while (1) {
        if (poll(pollFds, clientCnt + 1, -1) == -1) {
            unlink(SOCKET_PATH);
            perror("Poll failed");
            exit(-1);
        }

        for (int i = 1; i <= clientCnt; i++) {
            if (pollFds[i].revents & POLLIN) {
                size_t bytesRead = read(pollFds[i].fd, buffer, BUFSIZ);

                if (bytesRead <= 0) {
                    close(pollFds[i].fd);
                    pollFds[i] = pollFds[clientCnt];
                    pollFds[clientCnt].fd = 0;
                    pollFds[clientCnt].events = 0;
                    clientCnt--;
                    pollFds[0].events = POLLIN;

                }
                else {
                    for (size_t j = 0; j < bytesRead; j++) {
                        putc(toupper(buffer[j]), stdout);
                    }
                }
            }
        }

        if (pollFds[0].revents & POLLIN) {
            int clientFd;
            if ((clientFd = accept(socketFd, NULL, NULL)) == -1) {
                perror("Accept failed");
                unlink(SOCKET_PATH);
                exit(-1);
            }
            clientCnt++;
            pollFds[clientCnt].fd = clientFd;
            pollFds[clientCnt].events = POLLIN;
            if (clientCnt == MAX_CLIENTS) {
                pollFds[0].events = 0;
            }
        }
    }
}
