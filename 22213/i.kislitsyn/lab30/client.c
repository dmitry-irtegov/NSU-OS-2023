#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define SERVER_SOCK "server.sock"
#define MSGSIZE 20

int fd = -1;

void pipe_sig_handler();

int main()
{
    struct sockaddr_un addr;
    char msg[MSGSIZE];
    signal(SIGPIPE, pipe_sig_handler);

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed with socket creation");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_SOCK);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Connection is failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t red = 0;
    ssize_t sent;
    ssize_t to_send;
   
    while ((red = read(STDIN_FILENO, msg, MSGSIZE)) > 0)
    {
        
        to_send = red < MSGSIZE ? red : MSGSIZE;
        if ((sent = write(fd, msg, to_send)) != red)
        {
            fprintf(stderr, "Written not full message");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (sent == -1)
        {
            perror("Write error");
            close(fd);
            exit(EXIT_FAILURE);
        }

    }

    close(fd);

    if (red == -1)
    {
        perror("Read to buffer failed");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void pipe_sig_handler()
{
    if (fd != -1)
    {
        close(fd);
        write(STDERR_FILENO, "Writing to the server failure\n", 31);
    }

    _exit(EXIT_FAILURE);
}
