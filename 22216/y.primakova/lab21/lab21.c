#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

static int counter = 0;

void sigint(int sig) {
    if (counter == INT_MAX) {
        write(STDOUT_FILENO, "\nThe maximum counter has been reached.\n", 39);
        char buf[41];
        sprintf(buf, "\nThe sound was produced %d times", counter);
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(-1);
    }
    write(STDOUT_FILENO, "\a\n", 2);
    counter++;    
}

void sigquit(int sig) {
    char buf[41];
    sprintf(buf, "\nThe sound was produced %d times", counter);
    write(STDOUT_FILENO, buf, strlen(buf));
    exit(-1);
}

int main(int argc, char** argv) {
    if (sigset(SIGINT, sigint) == SIG_ERR) {
        perror("Fail to set SIGINT handler.");
        exit(-1);
    }
    if (sigset(SIGQUIT, sigquit) == SIG_ERR) {
        perror("Fail to set SIGQUIT handler.");
        exit(-1);
    }
    while (1) {}
}
