#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX_CLIENTS 510


char sigPipe;

void sigPipeCatch(int sig) {
    sigPipe = 1;
    signal(SIGPIPE, sigPipeCatch);
}

void closeClient(struct pollfd fds[], nfds_t clientCount, int clientNumber) {
    close(fds[(clientNumber << 1) - 1].fd);
    close(fds[clientNumber << 1].fd);

    fds[(clientNumber << 1) - 1] = fds[(clientCount << 1) - 1];
    fds[clientNumber << 1] = fds[clientCount << 1];

    fds[0].events = POLLIN;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Incorrect usage. Use: %s <listenPort> <addr> <broadcastPort>\n", argv[0]);
        return -1;
    }

    char *name = argv[2];
    int listenPort = atoi(argv[1]);
    int broadcastPort = atoi(argv[3]);
    if (!listenPort || !broadcastPort) {
        fprintf(stderr, "Incorrect port.\n");
        return -1;
    }

    char buffer[BUFSIZ];
    int proxyFd, clientFd, serverFd;
    struct sockaddr_in proxyAddr;

    if ((proxyFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    memset(&proxyAddr, 0, sizeof(proxyAddr));
    proxyAddr.sin_family = AF_INET;
    proxyAddr.sin_addr.s_addr = INADDR_ANY;
    proxyAddr.sin_port = htons(listenPort);

    if (bind(proxyFd, (struct sockaddr *) &proxyAddr, sizeof(proxyAddr)) == -1) {
        perror("Bind failed");
        exit(-1);
    }

    if (listen(proxyFd, MAX_CLIENTS) == -1) {
        perror("Listen error");
        exit(-1);
    }

    struct pollfd pollFds[(MAX_CLIENTS << 1) + 1];
    memset(pollFds, 0, sizeof(pollFds));

    pollFds[0].fd = proxyFd;
    pollFds[0].events = POLLIN;

    nfds_t clientCnt = 0;

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(broadcastPort);
    inet_pton(AF_INET, name, &serverAddr.sin_addr);

    signal(SIGPIPE, sigPipeCatch);

    while (1) {
        if (poll(pollFds, (clientCnt << 1) + 1, -1) == -1) {
            perror("Poll failed");
            exit(-1);
        }

        for (int i = 1; i <= (clientCnt << 1); i++) {
            short revents = pollFds[i].revents;
            if (revents & POLLERR
                || revents & POLLHUP
                || revents & POLLNVAL) {
                closeClient(pollFds, clientCnt, (i + 1) >> 1);
                clientCnt--;
                i = i + (i & 1) - 2;
                printf("Client was disabled\n");

            } else if (revents & POLLIN) {
                size_t bytesRead = read(pollFds[i].fd, buffer, BUFSIZ);

                sigPipe = 0;
                if (bytesRead > 0) {
                    if (i & 1) {
                        write(pollFds[i + 1].fd, buffer, bytesRead);
                    } else {
                        write(pollFds[i - 1].fd, buffer, bytesRead);
                    }
                }

                if ((bytesRead <= 0) || (sigPipe == 1)) {
                    closeClient(pollFds, clientCnt, (i + 1) >> 1);
                    clientCnt--;
                    i = i + (i & 1) - 2;
                    printf("Client was disabled\n");
                }
            }
        }

        if (pollFds[0].revents & POLLIN) {
            if ((clientFd = accept(proxyFd, NULL, NULL)) == -1) {
                perror("Accept failed");
                continue;
            }

            if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("Socket failed");
                continue;
            }

            if (connect(serverFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
                perror("Connect failed");
                continue;
            }

            clientCnt++;
            pollFds[(clientCnt << 1) - 1].fd = clientFd;
            pollFds[(clientCnt << 1) - 1].events = POLLIN;
            pollFds[clientCnt << 1].fd = serverFd;
            pollFds[clientCnt << 1].events = POLLIN;

            printf("Client was connected\n");

            if (clientCnt == MAX_CLIENTS) {
                pollFds[0].events = 0;
            }
        }
    }
}
