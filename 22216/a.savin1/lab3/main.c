#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {

    printf("RID: %d\nEUID: %d\n", getuid(), geteuid());

    FILE* file = fopen("file", "r");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Failed to open file \"file\" ");
    }

    if (setuid(getuid()) != 0) {
        perror("Failed to set new EUID");
        return -1;
    }

    printf("UID: %d\nEUID: %d\n", getuid(), geteuid());

    file = fopen("file", "r");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Failed to read file \"file\" ");
    }

    return 0;
}
