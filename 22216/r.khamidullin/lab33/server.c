#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <ctype.h>
#include <string.h>

#define MAX_CLIENTS 510
#define PORT 8081

int main() {
    char buffer[BUFSIZ];
    int socketFd;

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(-1);
    }

    if (listen(socketFd, MAX_CLIENTS) == -1) {
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
                    i--;

                } else {
                    for (size_t j = 0; j < bytesRead; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    write(pollFds[i].fd, buffer, bytesRead);
                }
            }
        }

        if (pollFds[0].revents & POLLIN) {
            int clientFd;
            if ((clientFd = accept(socketFd, NULL, NULL)) == -1) {
                perror("Accept failed");
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
