#include <locale.h>
#include <ctype.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>

char *socketPath = "./socket";

int clientReader(int descriptor) {
    char buffer[512];
    ssize_t reader;

    reader = read(descriptor, buffer, sizeof(buffer));
    if (reader < 0) {
        perror("Error while reading");
        return -1;
    } else {
        if (reader == 0) {
            return -1;
        } else {
            for (int i = 0; i < reader; i++) {
                putchar(toupper(buffer[i]));
            }
        }
    }
    return 0;
}

void sigHandler(int signo) {
    unlink(socketPath);
    exit(0);
}

void unlinker() {
    unlink(socketPath);
}

int descriptorsArray[FD_SETSIZE];
int countBusy = 0;

void addDescriptor(int desc) {
    descriptorsArray[countBusy] = desc;
    countBusy++;
    descriptorsArray[countBusy] = -1;
}

void deleteDescriptor(int desc) {
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (descriptorsArray[i] == desc) {
            for (int j = i; j < countBusy; j++) {
                descriptorsArray[j] = descriptorsArray[j + 1];
            }
            countBusy--;
            descriptorsArray[countBusy] = -1;
            return;
        }
    }
    perror("Something strange with descriptor\n");
}

int main() {
    setlocale(LC_ALL, "ru_RU.KOI8-R");
    signal(SIGINT, sigHandler);
    int serverDescriptor;
    struct sockaddr_un serverAddress;
    fd_set activeSet, readSet;
    
    if ((serverDescriptor = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Can't create socket");
        return -1;
    }
    
    for (int i = 0; i < FD_SETSIZE; i++) {
        descriptorsArray[i] = -1;
    }
    
    descriptorsArray[countBusy] = serverDescriptor;
    descriptorsArray[countBusy + 1] = -1;
    
    serverAddress.sun_family = AF_UNIX;
    memset(serverAddress.sun_path, 0, sizeof(serverAddress.sun_path));
    strncpy(serverAddress.sun_path, socketPath, sizeof(serverAddress.sun_path) - 1);
    unlink(socketPath);
    
    if (bind(serverDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error while binding");
        return -1;
    }
    
    if (atexit(unlinker) != 0) {
        perror("Error while atexit");
        return -1;
    }
    
    if (listen(serverDescriptor, 50) == -1) {
        perror("Error while listening");
        return -1;
    }
    
    FD_ZERO(&activeSet);
    FD_SET(serverDescriptor, &activeSet);
    addDescriptor(serverDescriptor);
    
    while (1) {
        readSet = activeSet;
        if (select(FD_SETSIZE, &readSet, NULL, NULL, NULL) < 0) {
            perror("Error while selecting");
            return -1;
        }
        
        int i = 0;
        while (descriptorsArray[i] != -1) {
            if (FD_ISSET(descriptorsArray[i], &readSet)) {
                if (descriptorsArray[i] == serverDescriptor) {
                    int currentClient = accept(serverDescriptor, NULL, NULL);
                    if (currentClient < 0) {
                        perror("Error while accepting");
                    }
                    addDescriptor(currentClient);
                    FD_SET(currentClient, &activeSet);
                } else {
                    if (clientReader(descriptorsArray[i]) < 0) {
                        close(descriptorsArray[i]);
                        FD_CLR(descriptorsArray[i], &activeSet);
                        deleteDescriptor(descriptorsArray[i]);
                    }
                }
            }
            i++;
        }
    }
}
