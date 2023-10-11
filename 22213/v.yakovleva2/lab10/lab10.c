#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return 1;
    }

    pid_t pid = fork();
    switch (pid) {
        case -1: return 1;
        case 0:
        {
            execvp(argv[1], argv + 1);
            return 0;
        }
        default:
        {
            int stat_loc;
            wait(&stat_loc);

            if (WIFEXITED(stat_loc)) {
                printf("\nChild process finished with exit code %d\n",
                       WEXITSTATUS(stat_loc));
            }
        }
    }

    return 0;
}
