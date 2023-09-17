#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    int opt;
    int s_opts = argc;
    int s_args = argc;
    int* opts = malloc(s_opts * sizeof(int));
    char** args = malloc(s_args * sizeof(char*));
    int i_opts = 0;
    int i_args = 0;

    while ((opt = getopt(argc, argv, "ispuU:cC:dvV:")) != EOF) {
        if (opt == 'U' || opt == 'C' || opt == 'V') {
            if (s_opts <= i_opts) {
                s_opts *= 2;
                int* ptr = realloc(opts, s_opts * sizeof(int));
                if (ptr == NULL) {
                    perror("");
                    break;
                }
                else
                    opts = ptr;
            }
            opts[i_opts++] = opt;
            if (s_args <= i_args) {
                s_args *= 2;
                char** ptr = realloc(args, s_args * sizeof(char*));
                if (ptr == NULL) {
                    perror("");
                    break;
                }
                else
                    args = ptr;
            }
            args[i_args++] = optarg;
        }
        else if (opt == 'i' || opt == 's' || opt == 'p' || opt == 'u' ||
                 opt == 'c' || opt == 'd' || opt == 'v') {
            if (s_opts <= i_opts) {
                s_opts *= 2;
                int* ptr = realloc(opts, s_opts * sizeof(int));
                if (ptr == NULL) {
                    perror("");
                    break;
                }
                else
                    opts = ptr;
            }
            opts[i_opts++] = opt;
        }
    }

    i_args--;
    i_opts--;
    for (; i_opts >= 0; i_opts--) {
        opt = opts[i_opts];
        switch (opt) {
            case 'i':
                printf("Real User ID: %d\n", getuid());
                printf("Effective User ID: %d\n", geteuid());
                printf("Real Group ID: %d\n", getgid());
                printf("Effective Group ID: %d\n\n", getegid());
                break;
            case 's':
                setpgid(0, 0);
                break;
            case 'p': {
                int pid = getpid();
                int pgid = getpgid(pid);
                printf("Process ID: %d\n", pid);
                printf("Parent Process ID: %d\n", getppid());
                printf("Process Group ID: %d\n", pgid);
                printf("\n");
                break;
            }
            case 'u': {
                struct rlimit limit;
                getrlimit(RLIMIT_FSIZE, &limit);
                printf("ulimit = %ju\n\n", (uintmax_t)limit.rlim_max);
                break;
            }
            case 'U': {
                struct rlimit limit;
                errno = 0;
                limit.rlim_max = strtol(args[i_args--], NULL, 10);
                if ((limit.rlim_max == 0 || limit.rlim_max == LONG_MIN || limit.rlim_max == LONG_MAX) && errno != 0)
                    perror("");
                else {
                    if (limit.rlim_cur > limit.rlim_max)
                        limit.rlim_cur = limit.rlim_max;
                    if (setrlimit(RLIMIT_FSIZE, &limit) == -1)
                        perror("");
                }
                break;
            }
            case 'c': {
                struct rlimit limit;
                getrlimit(RLIMIT_CORE, &limit);
                printf("The maximum size of a core file is %ju bytes\n\n", (uintmax_t)limit.rlim_max);
                break;
            }
            case 'C': {
                struct rlimit limit;
                errno = 0;
                limit.rlim_max = strtol(args[i_args--], NULL, 10);
                if ((limit.rlim_max == 0 || limit.rlim_max == LONG_MIN || limit.rlim_max == LONG_MAX) && errno != 0)
                    perror("");
                else {
                    if (limit.rlim_cur > limit.rlim_max)
                        limit.rlim_cur = limit.rlim_max;
                    if (setrlimit(RLIMIT_CORE, &limit) == -1)
                        perror("");
                }
                break;
            }
            case 'd': {
                char *DirName = malloc(PATH_MAX);
                getcwd(DirName, PATH_MAX);
                printf("The current working directory is %s\n\n", DirName);
                free(DirName);
                break;
            }
            case 'v':
                printf("PATH = %s\n", getenv("PATH"));
                printf("TERM = %s\n", getenv("TERM"));
                printf("TZ = %s\n", getenv("TZ"));
                printf("HOME = %s\n", getenv("HOME"));
                printf("USER = %s\n\n", getenv("USER"));
                break;
            case 'V': {
                char *name = strtok(args[i_args--], "=");
                char *value = strtok(NULL, "=");
                setenv(name, value, 1);
            }
        }
    }
    free(opts);
    free(args);
    return  0;
}
