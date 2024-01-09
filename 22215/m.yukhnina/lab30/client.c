#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SERVER_SOCK "socket.sock"
#define BUF_SIZE 20


//пишет
int main(int argc, char *argv[]) {

    int read_chars = 0, written_bytes, curr_write, need_write;

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("SOCK CREATE FAILURE");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_SOCK, sizeof(addr.sun_path)-1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("CONNECTION FAILURE");
        return -1;
    }

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    do {
        read_chars = read(STDIN_FILENO, buf, BUF_SIZE);
        if (read_chars == -1) {
            perror("Error while reading line from user");
            return -1;
        }
        written_bytes = 0;
        curr_write = 0;
        do {
            need_write = read_chars - written_bytes;
            curr_write = write(fd, buf + written_bytes, need_write);
            if (curr_write == -1) {
                perror("ERROR WRITE ERROR");
                return -1;
            }
            written_bytes += curr_write;
        } while (written_bytes < read_chars);

    } while (read_chars > 0);

    close(fd);
    return 0;
}