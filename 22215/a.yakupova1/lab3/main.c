
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char **argv){

    if (argc != 2) {
	perror("File name is missing");
	exit(1);
    }
    printf("real user ID       = %d\n", getuid());
    printf("effective user ID  = %d\n", geteuid());
    printf("===========================\n");
    FILE *f;
    f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror("This file can't be opened");
    }
    fclose(f);

    setuid(getuid());
    printf("new results:\n");
    printf("real user ID       = %d\n", getuid());
    printf("effective user ID  = %d\n", geteuid());
    f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror("This file can't be opened");
    }
    fclose(f);

    return 0;
}
