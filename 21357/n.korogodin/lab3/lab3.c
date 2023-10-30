#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n\n", geteuid());

    FILE* stream = fopen("lab3file.txt", "r");
    printf("File opened\n\n");
    printf("Effective User ID = %d\n", geteuid());
    printf("Real User ID = %d\n\n", getuid());
    printf("file closed\n\n");
   
    setuid(geteuid());
    if (stream != NULL) {
        fclose(stream);
    }
    else {
        perror("failed to open file");
    }

    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n\n", geteuid());
    stream = fopen("lab3file.txt", "r");

    if (stream != NULL) {
        fclose(stream);
    }
    else {
        perror("failed to open file");
    }

}

