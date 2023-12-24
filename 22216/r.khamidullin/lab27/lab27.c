#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>", argv[0]);
        return -1;
    }

    int file;
    if ((file = open(argv[1], O_RDONLY)) == -1) {
        perror("Open failed");
        return -1;
    }

    FILE *wc;
    if ((wc = popen("wc -l", "w")) == NULL) {
        perror("Popen failed");
        return -1;
    }

    char buffer[BUFSIZ];
    char prevIsNewLine = 1;
    size_t readBytes;
    while ((readBytes = read(file, buffer, BUFSIZ)) > 0) {
        for (int i = 0; i < readBytes; i++) {
            if (buffer[i] == '\n') {
                if (prevIsNewLine) {
                    fputc('\n', wc);
                }
                prevIsNewLine = 1;
            } else {
                prevIsNewLine = 0;
            }
        }
    }

    pclose(wc);
    close(file);
    return 0;
}
