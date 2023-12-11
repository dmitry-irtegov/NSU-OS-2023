#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Real UID: %d;  Effective UID: %d\n", getuid(), geteuid());

    FILE *file = fopen("file", "r");
    if (file == NULL) {
        perror("Failed to open the file");
    } 
    else {
        fclose(file);
    }

    setuid(getuid());
    printf("Real UID: %d;  Effective UID: %d\n", getuid(), geteuid());

    file = fopen("file", "r");
    if (file == NULL) {
        perror("Failed to open the file");
    } 
    else {
        fclose(file);
    }

    return 0;
}
