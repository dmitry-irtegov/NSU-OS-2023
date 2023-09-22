#include <stdio.h>
#include <unistd.h>

void first_and_second() {
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();
    
    printf("Real user's id: %d\n", real_uid);
    printf("Effective user's id: %d\n", effective_uid);

    FILE* file = fopen("someFile.txt", "r");
    
    if (file != NULL) {
        fclose(file);
    } else {
        perror("error with file");
    }    
}

int main(){
    first_and_second();

    if (setuid(getuid()) != 0) {
        perror("effective user's id changing error");
        return 1;
    }

    first_and_second();

    return 0;
}
