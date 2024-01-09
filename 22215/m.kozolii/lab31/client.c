#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
char *path = "./socket";

int main()
{
        struct sockaddr_un address;
        char buff[BUFSIZ];
        int descriptor;
        if ((descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        {
            perror("Can't create socket");
            return -1;
        }
        address.sun_family = AF_UNIX;
        memset(address.sun_path, 0, sizeof(address.sun_path));
        strncpy(address.sun_path, path, sizeof(address.sun_path) - 1);
        if (connect(descriptor, (struct sockaddr *)&address, sizeof(address)) == -1)
        {
            perror("error while connecting");
            return -1;
        }
        memset(buff, 0, BUFSIZ);
        ssize_t count;
        while ((count = read(STDIN_FILENO, buff, BUFSIZ)) > 0)
        {
                if (write(descriptor, buff, count) != count && count == -1)
                {
                    perror("Error while writing");
                    return -1;
                }
        }
        close(descriptor);
        return 0;
}