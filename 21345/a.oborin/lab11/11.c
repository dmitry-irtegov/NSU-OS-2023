#include <stdio.h>
extern char** environ;
int main(int argc, char* argv[]) {
    char** p;
    int n;
    printf("My environment variables are: \n");
    for (p = environ; *p; p++)
        printf(" %s \n", *p);
    printf("My input parameters are: \n");
    for (n = 0; n < argc; n++)
        printf("'%s' \n", argv[n]);
    return 0;
}
