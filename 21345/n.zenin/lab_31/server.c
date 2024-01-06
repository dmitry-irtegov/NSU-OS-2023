#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#define MAX_CLIENTS_NUMBER 5

char *socket_path = "./server_socket";
int socket_fd;

void sigint_handler(int sign)
{
    signal(SIGINT, sigint_handler);
    if (access(socket_path, F_OK) == 0)
    {
        unlink(socket_path);
    }
    close(socket_fd);
    exit(0);
}

void max_connection_number_check(int connection_number, fd_set *active_fd_set)
{
    if (connection_number >= MAX_CLIENTS_NUMBER)
    {
        FD_CLR(socket_fd, active_fd_set);
        return;
    }
    FD_SET(socket_fd, active_fd_set);
}

void process_new_connection(fd_set *active_fd_set, int *connection_number, int *max_fd)
{
    int new_connection = accept(socket_fd, NULL, NULL);
    if (new_connection == -1)
    {
        perror("Error acception");
    }
    else
    {
        printf("New connection %d\n", new_connection);
        FD_SET(new_connection, active_fd_set);
        *connection_number += 1;
        *max_fd = new_connection > *max_fd ? new_connection : *max_fd;
    }
}

void write_toupper(int readen, char buffer[])
{
    for (int index = 0; index < readen; ++index)
    {
        buffer[index] = toupper(buffer[index]);
    }
    printf("%s", buffer);
}

void disconnect_connection(int *connection_number, int *max_fd, int fd, int prev_processing_fd)
{
    if (*max_fd == fd)
    {
        *max_fd = prev_processing_fd;
    }
    printf("Disconnect %d\n", fd);
    *connection_number -= 1;
}

void process_connections(int *max_fd, fd_set *read_fd_set, fd_set *active_fd_set, int *prev_processing_fd,
                         int *connection_number, char buffer[])
{
    for (int fd = 0; fd <= *max_fd; ++fd)
    {
        if (!FD_ISSET(fd, read_fd_set))
        {
            continue;
        }
        if (socket_fd == fd)
        {
            process_new_connection(active_fd_set, connection_number, max_fd);
            continue;
        }
        int readen = read(fd, buffer, BUFSIZ);
        if (readen != BUFSIZ)
        {
            buffer[readen] = '\0';
        }
        if (readen > 0)
        {
            write_toupper(readen, buffer);
            *prev_processing_fd = fd;
            continue;
        }
        if (readen < 0)
        {
            perror("Error reading");
        }
        if (readen == 0)
        {
            disconnect_connection(connection_number, max_fd, fd, *prev_processing_fd);
        }
        FD_CLR(fd, active_fd_set);
        close(fd);
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_un server_socket_addr;
    fd_set active_fd_set, read_fd_set;
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sigint_handler);

    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        exit(1);
    }

    memset(&server_socket_addr, 0, sizeof(server_socket_addr));
    server_socket_addr.sun_family = AF_UNIX;
    strncpy(server_socket_addr.sun_path, socket_path, sizeof(server_socket_addr.sun_path) - 1);
    if (access(socket_path, F_OK) == 0)
    {
        unlink(socket_path);
    }

    if (bind(socket_fd, (struct sockaddr *)&server_socket_addr, sizeof(server_socket_addr)) == -1)
    {
        perror("Bind error");
        close(socket_fd);
        exit(1);
    }

    if (listen(socket_fd, 5) == -1)
    {
        perror("Listen error");
        close(socket_fd);
        if (access(socket_path, F_OK) == 0)
        {
            unlink(socket_path);
        }
        exit(1);
    }

    FD_ZERO(&active_fd_set);
    FD_ZERO(&read_fd_set);
    FD_SET(socket_fd, &active_fd_set);
    int connection_number = 0;
    int max_fd = socket_fd;
    int prev_processing_fd = 0;
    char buffer[BUFSIZ];
    while (1)
    {
        max_connection_number_check(connection_number, &active_fd_set);
        read_fd_set = active_fd_set;
        if (select(max_fd + 1, &read_fd_set, NULL, NULL, NULL) == -1)
        {
            perror("Select error");
            if (access(socket_path, F_OK) == 0)
            {
                unlink(socket_path);
            }
            close(socket_fd);
            break;
        }
        process_connections(&max_fd, &read_fd_set, &active_fd_set, &prev_processing_fd, &connection_number, buffer);
    }
    return 0;
}
