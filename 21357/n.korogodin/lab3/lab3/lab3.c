#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n\n", geteuid());

    FILE* stream = fopen("lab3file", "r");

    if (stream != NULL) {
        fclose(stream);
    }
    else {
        perror("failed to open file");
    }
    seteuid(getuid());

    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n\n", geteuid());

    if (stream != NULL) {
        fclose(stream);
    }
    else {
        perror("failed to open file");
    }

}
