#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
char *socket_path = "./server_socket";
int socket_fd;

int try_connect(struct sockaddr_un *server_socket_addr)
{
    if (access(socket_path, F_OK) == 0)
    {
        write(STDOUT_FILENO, "Connection...\n", 15);
        if (connect(socket_fd, (struct sockaddr *)server_socket_addr, sizeof(*server_socket_addr)) == -1)
        {
            close(socket_fd);
            if(errno == EISCONN)
            {
                socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
                if(connect(socket_fd, (struct sockaddr *)server_socket_addr, sizeof(*server_socket_addr)) != -1)
                {
                    return 1;
                }
            }
            perror("Error connection");
            exit(1);
        }
        return 1;
    }
    return 0;
}

int main()
{
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Error socket");
        exit(1);
    }
    signal(SIGPIPE, SIG_IGN);
    struct sockaddr_un server_socket_addr;
    memset(&server_socket_addr, 0, sizeof(server_socket_addr));
    server_socket_addr.sun_family = AF_UNIX;
    strncpy(server_socket_addr.sun_path, socket_path, sizeof(server_socket_addr.sun_path) - 1);
    if(try_connect(&server_socket_addr) == 0)
    {
        write(STDERR_FILENO, "Error connection\n", 18);
        exit(1);
    }
    int readen;
    int written;
    char buffer[BUFSIZ];
    write(STDOUT_FILENO, "Input message:", 15);
    while ((readen = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
    {
        if(readen != BUFSIZ)
        {
            buffer[readen] = '\0';
        }
        if ((written = write(socket_fd, buffer, readen)) < 0)
        {
            if (errno == EPIPE)
            {
                perror("Connection error");
                if(try_connect(&server_socket_addr))
                {
                    write(STDOUT_FILENO, "Connection establish\n", 22);
                    write(socket_fd, buffer, readen);
                    continue;
                }
            }
            else
            {
                perror("Write error");
                close(socket_fd);
            }
        }
    }

    close(socket_fd);
    return 0;
}
