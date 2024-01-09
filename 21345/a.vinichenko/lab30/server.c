#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int listener;
    struct sockaddr_un addr;
    char buf[BUFSIZ];
    int bytes_read;
    const char SOCK_PATH[9] = "./socket";

    if((listener = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("Error open socket");
        return -1;
    }

    int enable = 1;
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("Error change socket options");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Error bind to addres");
        return -1;
    }

    listen(listener, 1);

    int client = accept(listener, NULL, NULL);

    if(client < 0)
    {
        perror("Error accept connection");
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    while((bytes_read = recv(client, buf, sizeof(buf), 0)) > 0)
    {
        for(int i = 0; i < bytes_read; i++)
        {
    	    putchar(toupper(buf[i]));
        }
    }

    if(close(client) < 0)
    {
        perror("Error close client connection");
        return -1;
    }

    if(close(listener) < 0)
    {
        perror("Error close server");
        return -1;
    }

    unlink(SOCK_PATH);
    return 0;
}
