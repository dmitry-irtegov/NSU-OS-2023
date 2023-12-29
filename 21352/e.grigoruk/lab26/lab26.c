#include <stdio.h>
#include <ctype.h>
#include <wait.h>

int main(int argc, char** argv) {
    FILE* pipe = popen("tr [:lower:] [:upper:]", "w");
    if (pipe == NULL) {
        perror("popen() error");
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen() error");
        pclose(pipe);
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int result = fputs(buffer, pipe);
        if (result == -1) {
            perror("fputs() error");
            pclose(pipe);
            fclose(file);
            return 1;
        }
    }

    int status = pclose(pipe);
    if (status == -1) {
        perror("pclose() error");
        fclose(file);
        return 1;
    }
    else {
        if (WIFEXITED(status)) {
            printf("Child terminated normally, status = %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) {
            printf("Killed by signal with number %d\n", WTERMSIG(status));
        }
        else if (WIFSTOPPED(status)) {
            printf("Stopped by signal %d\n", WSTOPSIG(status));
        }
    }
    fclose(file);
    return 0;
}

