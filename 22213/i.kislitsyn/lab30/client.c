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
void int_sig_handler();

int main()
{
    struct sockaddr_un addr;
    char msg[MSGSIZE];
    signal(SIGPIPE, pipe_sig_handler);
    signal(SIGINT, int_sig_handler);

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
    while (1)
    {
        if ((red = read(STDIN_FILENO, msg, MSGSIZE)) == -1)
        {
            perror("Read to buffer failed");
            close(fd);
            exit(EXIT_FAILURE);
        }
        ssize_t to_send = red < MSGSIZE ? red : MSGSIZE;

        if((write(fd, msg, to_send)) == -1)
        {
            perror("Write to server failed");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
}

void pipe_sig_handler()
{
    if (fd != -1)
    {
        close(fd);
        write(STDERR_FILENO, "Writing to the server failure\n", 31);
    }

    exit(EXIT_FAILURE);
}

void int_sig_handler()
{
    if (fd != -1)
    {
        close(fd);
    }
    write(STDOUT_FILENO, "\nTranslation is finished\n", 22);

    exit(EXIT_SUCCESS);
}