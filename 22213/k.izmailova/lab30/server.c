#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int sfd, cfd; // sfd - server socket descriptor, cfd - client socket descriptor
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_un addr; // UNIX domain socket address
    memset(&addr, 0, sizeof(addr)); // Initialize memory with zero values, ensuring the address structure is initialized before setting specific values.
    addr.sun_family = AF_UNIX;
    char* socketPath = "./socket";
    strcpy(addr.sun_path, socketPath);
    unlink(socketPath);

    // Bind the network address of the computer to the socket identifier
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(1);
    }

    /*
    * Accept connections
    * After binding to the address and before any client can connect to the created socket,
    * the server needs to start accepting connections.
    * The XSI specification defines the minimum upper limit on the queue length as 5.
    */
    if (listen(sfd, 5) == -1) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        /*
        * When the server receives a connection request from a client, it needs
        * to create a new socket to handle the new connection.
        * When a client connects, accept returns a new file descriptor (cfd) representing the connection with the client.
        * If an error occurs, the code prints an error message using perror and exits the program with an error code.
        */
        if ((cfd = accept(sfd, NULL, NULL)) == -1) {
            perror("accept error");
            exit(1);
        }

        printf("the connection is established\n");
        int rc;
        char buf[BUFSIZ];

        // Read data from the client socket into a buffer and convert characters to uppercase
        while ((rc = read(cfd, buf, BUFSIZ)) > 0) {
            for (int i = 0; i < rc; ++i) {
                putchar(toupper(buf[i]));
            }
        }
        if (rc == -1) {
            perror("read error");
            exit(1);
        }
        if (rc == 0) {
            printf("the connection was closed\n");
            close(cfd);
            return 0;
        }
    }
}
//что будет после запуска сервера и его закрытия если запустить другую программу которой понадобится сокет