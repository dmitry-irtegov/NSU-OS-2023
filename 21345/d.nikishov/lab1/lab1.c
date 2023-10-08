#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <ulimit.h>
#include <unistd.h>

extern char** environ;

int main(int argc, char* argv[]) {
    char options[] = "ispuU:cC:dvV:";
    int opt;
    long new_ulimit = 0;
    long new_core_size = 0;
    char* new_env_var = NULL;

    uid_t real_uid;
    uid_t efficie_uid;
    gid_t real_gid;
    gid_t efficie_gid;

    pid_t pid;
    pid_t ppid;
    pid_t pgid;

    struct rlimit core_lim;
    struct rlimit new_rlim;

    long size;
    char* cwd;

    long ulimit_value;

    if (argc == 1) {
        fprintf(stderr, "Mistake: no options are specified. Usage: %s [options]\n", argv[0]);
        return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, options)) != EOF) {
        switch (opt) {
        case 'i':
            real_uid = getuid();
            efficie_uid = geteuid();
            real_gid = getgid();
            efficie_gid = getegid();
            printf("Real UID: %d\n", real_uid);
            printf("Effective UID: %d\n", efficie_uid);
            printf("Real GID: %d\n", real_gid);
            printf("Effective GID: %d\n", efficie_gid);
            break;
        case 's':
            pid = getpid();
            if (setpgid(pid, pid) == -1) {
                perror("Failed to set group");
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            pid = getpid();
            ppid = getppid();
            pgid = getpgrp();
            printf("PID of the current process: %d\n", pid);
            printf("PPID of the parent process: %d\n", ppid);
            printf("PGID of the process group: %d\n", pgid);
            break;
        case 'u':
            ulimit_value = ulimit(UL_GETFSIZE);
            if (ulimit_value == -1) {
                perror("Error receiving ulimit");
                exit(EXIT_FAILURE);
            }
            printf("ulimit value: %ld\n", ulimit_value);
            break;
        case 'U':
            new_ulimit = atol(optarg);
            if (new_ulimit < 0) {
                fprintf(stderr, "Error: Invalid value of the new ulimit\n");
                exit(EXIT_FAILURE);
            }
            if (ulimit(UL_SETFSIZE, new_ulimit) == -1) {
                perror("Error when changing ulimit");
                exit(EXIT_FAILURE);
            }
            printf("ulimit value successfully changed to %ld\n", new_ulimit);
            break;
        case 'c':
            if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                perror("failed to get core limit");
            printf("hard core limit is: %lu\nsoft core limit is: %lu\n", core_lim.rlim_max, core_lim.rlim_cur);
            break;
        case 'C':
            new_core_size = atol(optarg);
            new_rlim.rlim_cur = new_core_size;
            new_rlim.rlim_max = new_core_size;
            if (setrlimit(RLIMIT_CORE, &new_rlim) == -1) {
                perror("Error when changing the core-file size limit");
                exit(EXIT_FAILURE);
            }
            printf("The size of the core-file has been successfully changed to %lld bytes\n", (long long)new_rlim.rlim_cur);
            break;
        case 'd':
            size = pathconf(".", _PC_PATH_MAX);
            cwd = getcwd(NULL, (size_t)size);
            if (cwd == NULL) {
                perror("Error getting the current working directory");
                exit(EXIT_FAILURE);
            }
            printf("Current working directory: %s\n", cwd);
            break;
        case 'v':
            for (char** env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
            }
            break;
        case 'V':
            new_env_var = optarg;
            if (putenv(new_env_var) != 0) {
                perror("Error setting environment variable");
                exit(EXIT_FAILURE);
            }
            printf("Environment variable successfully set or changed: %s\n", new_env_var);
            break;
        default:
            fprintf(stderr, "Usage: %s %s\n", argv[0], options);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
