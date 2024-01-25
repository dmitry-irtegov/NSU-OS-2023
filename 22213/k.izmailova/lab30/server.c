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
    addr.sun_family = AF_UNIX; // Sets the address family for the UNIX socket.
    char* socketPath = "./socket"; // Defines the path to the file that will be used as the address for the UNIX socket
    strcpy(addr.sun_path, socketPath); // Copies the socket file path into the sockaddr_un structure
    unlink(socketPath); // Deletes the previous socket file with the specified path to avoid conflicts.

    // Bind the network address of the computer to the socket identifier
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sfd);
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
        close(sfd);
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
            close(sfd);
            perror("accept error");
            exit(1);
        }

        printf("the connection is established\n");
        int rc; // Variable to store the result of the read operation.
        char buf[BUFSIZ]; // Buffer to store data.

        // Infinite loop for reading data from the client socket (cfd) into the buffer (buf).
        // The read function returns the number of bytes successfully read from the socket.
        while ((rc = read(cfd, buf, BUFSIZ)) > 0) {
            // converting to uppercase and print to the standard output using putchar.
            for (int i = 0; i < rc; ++i) {
                putchar(toupper(buf[i])); //как оптимизировать. 
            }
        }

        // If the result of the read operation is -1, it indicates an error during reading.
        if (rc == -1) {
            close(sfd);
            close(cfd);
            perror("read error");
            exit(1);
        }

        // If the result of the read operation is 0, it means the connection was closed by the client.
        if (rc == 0) {
            printf("the connection was closed\n");
            close(cfd);
            close(sfd);
            return 0;
        }
    }
}

//закрывать дескрипторы во время ошибок. что такое сокеты и как они работают