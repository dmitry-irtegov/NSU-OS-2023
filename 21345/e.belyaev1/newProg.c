#include <stdio.h>
extern char** environ;

int main(int argc, char* argv[]){
    char** p;
    int i;
    printf("Input parameters are:\n");
    for (i = 0; i < argc; i++)
        printf(" '%s'\n", argv[i]);
    printf("Environment variables are:\n");
    for (p = environ; *p != 0; p++)
        printf(" %s\n", *p);
    return 0;
}
