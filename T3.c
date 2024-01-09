#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
int main (int argc, char **argv)
{
    printf("Real UID\t= %d\n", getuid());
    printf("Effective UID\t= %d\n", geteuid());
    if (argc != 2)
    {
        perror("No file name\n");
        exit(1);
    }
    FILE * file;
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("File not open");
    }
    fclose(file);
    setuid(getuid());
    printf("after using setuid \n");
    printf("Real UID\t= %d\n", getuid());
    printf("Effective UID\t= %d\n", geteuid());
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("File not open");
    }
    fclose(file);
    return 0;
}
