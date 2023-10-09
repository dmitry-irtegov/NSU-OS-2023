#include <stdio.h>
#include <errno.h>
#include <ucred.h>
#include <unistd.h>
#include <stdlib.h>

void check_file(const char* file_name) {
    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    if (fclose(file)) {
        perror("Failed to close file");
    };
}

int main(int argc, char *argv[]) {
    const char *file_name = "test_file.txt";

    printf("Effective user id: %d. Real user id: %d.\n", geteuid(), getuid());

    check_file(file_name);

    uid_t my_uid = getuid();
    if (setuid(my_uid)) {
        perror("Failed to set effective user id");    
    };

    printf("Effective uid: %d. Real uid: %d.\n", geteuid(), getuid());

    check_file(file_name);

    exit(0);
}
