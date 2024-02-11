#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define SERVER_SOCK "server.sock"
#define MSGSIZE 20

int main()
{
    int fd;
    
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed with socket creation");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    char msg[MSGSIZE];
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_SOCK);

    unlink(SERVER_SOCK);

    if ((bind(fd, (struct sockaddr *)&addr, sizeof(addr))) < 0)
    {
        close(fd);
        perror("Failed with binding");
        exit(EXIT_FAILURE);
    }


    if (listen(fd, 1) < 0)
    {
        close(fd);
        perror("Listening failure");
        unlink(SERVER_SOCK);
        exit(EXIT_FAILURE);
    }

    int accepted;
    if ((accepted = accept(fd, NULL, NULL)) == -1)
    {
        close(fd);
        perror("Accept failed");
        unlink(SERVER_SOCK);
        exit(EXIT_FAILURE);
    }

    unlink(SERVER_SOCK);

    int len;
    while ((len = read(accepted, msg, MSGSIZE)) > 0)
    {
        for (int i = 0; i < len; i++)
        {
            msg[i] = toupper(msg[i]);
        }

        if ((write(STDOUT_FILENO, msg, len)) == -1)
        {
            close(accepted);
            close(fd);
            perror("Failed with writing");
            exit(EXIT_FAILURE);
        }
    }

    if (len == -1)
    {
        perror("Failed with reading");
        close(fd);
        close(accepted);
        exit(EXIT_FAILURE);
    }

    printf("Connection is closed\n");
    close(accepted);
    close(fd);
    exit(EXIT_SUCCESS);
}