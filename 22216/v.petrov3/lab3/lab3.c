#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int check_file_and_print_uid(uid_t uid, uid_t euid){
    printf("effective id: %u\nreal id: %u\n", uid, euid);
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        return -1;
    }
    fclose(file);
    return 0;
}

int main () {
    uid_t uid, euid;
    uid = getuid(); euid = geteuid();
    //first check
    if (check_file_and_print_uid(uid, euid) == -1) {
        perror("check failed");
        exit(1);
    }

    int status = setuid(getuid());
    if (status < 0) {
        perror("Set the UID");
        exit(1);
    }
    uid = getuid(); euid = geteuid();
    //second check
    if (check_file_and_print_uid(uid, euid) == -1) {
        perror("check failed");
        exit(1);
    }

    return 0;
}
