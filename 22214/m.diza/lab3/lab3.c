#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void first_and_second_points(){
    printf("%d %d\n", getuid(), geteuid());
    FILE * file = fopen("file", "r");
    if (file == NULL) perror("");
    else fclose(file);
}

int main() {
    first_and_second_points();
    setuid(getuid());
    first_and_second_points();
    return 0;
}
