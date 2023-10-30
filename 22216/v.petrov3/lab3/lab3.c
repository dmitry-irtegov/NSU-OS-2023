#include <stdio.h>
#include <unistd.h>

void check_file_and_print_uid(uid_t uid, uid_t euid){
    printf("effective id: %u\nreal id: %u\n", uid, euid);
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        perror("open file");
    }
    fclose(file);
}

int main () {
    uid_t uid, euid;
    uid = getuid(); euid = geteuid();
    //first check
    check_file_and_print_uid(uid, euid);

    int status = setuid(getuid());
    if (status < 0) {
        perror("Set the UID");
    }
    uid = getuid(); euid = geteuid();
    //second check
    check_file_and_print_uid(uid, euid);

    return 0;
}