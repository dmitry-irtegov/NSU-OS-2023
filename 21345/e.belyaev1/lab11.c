#include <stdio.h>
#include <unistd.h>
extern char** environ;

int execpve(const char* file, char* argv[], char* envp[]) {
    if (file == NULL) {
        fprintf(stderr, "NULL pointer to file\n");
        return -1;
    }
    char** savedEnviron = environ;
    environ = envp;
    execvp(file, argv);
    environ = savedEnviron;
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return 0;
    }
    char* nenv[] = { "PATH=/bin/:.", NULL };
    execpve(argv[1], &argv[1], nenv);
    perror("execpve error");
    return 0;
}
