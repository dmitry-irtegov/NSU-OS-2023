#include <unistd.h>
#include <stdio.h>

void printUIDs() {
    printf("uid: %d, euid: %d\n", getuid(), geteuid());
}

void tryToOpen(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open the file\n");
        return;
    }

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Filename is not specified\n");
        return 1;
    }

    printUIDs();
    tryToOpen(argv[1]);

    setuid(getuid());

    printUIDs();
    tryToOpen(argv[1]);

    return 0;
}