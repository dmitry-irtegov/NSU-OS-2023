#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    uid_t  uid, euid;
    uid = getuid();
    euid = geteuid();
    printf("  pw_ruid   : %d\n", uid);
    printf("  pw_euid   : %d\n", euid);

    if ((file = fopen("text.txt", "r")) == NULL) {
        perror("fopen: ");
        exit(1);
    }

    setuid(uid);

    printf("  pw_ruid   : %d\n", uid);
    printf("  pw_euid   : %d\n", euid);
    exit(0);
}
