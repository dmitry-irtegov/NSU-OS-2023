#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>

int main()
{
    char buf[BUFSIZ];
    int sock;
    struct sockaddr_un addr;
    const char SOCK_PATH[9] = "./socket";

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Can't socket");
        return - 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Can't Connect");
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    size_t read_count;
    while ((read_count = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
    {
        if(write(sock, buf, read_count) != read_count)
        {
            perror("Can't write");
            return -1;
        }
    }
    if(close(sock) != 0)
    {
	perror("Can't to close socket");
	return -1;
    }
    return 0;
}
