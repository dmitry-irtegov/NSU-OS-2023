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
    rlim_t new_core_size = 0;
    char* new_env_var = NULL;

    uid_t real_uid, efficie_uid;
    gid_t real_gid, efficie_gid;
    pid_t pid, ppid, pgid;

    struct rlimit core_lim, new_rlim;

    char* cwd = NULL;

    long ulimit_value;

    if (argc == 1) {
        fprintf(stderr, "Mistake: no options are specified. Usage: %s [options]\n", argv[0]);
        return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, options)) != -1) {
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
                continue;
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
                continue;
            }
            printf("ulimit value: %ld\n", ulimit_value * 512);
            break;
        case 'U':
            new_ulimit = atol(optarg);
            if (new_ulimit < 0 || new_ulimit % 512 != 0) {
                fprintf(stderr, "Error: Invalid value of the new ulimit\n");
                continue;
            }
            if (ulimit(UL_SETFSIZE, new_ulimit % 512) == -1) {
                perror("Error when changing ulimit");
                continue;
            }
            printf("Ulimit value successfully changed to %ld\n", new_ulimit);
            break;
        case 'c':
            if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                perror("Failed to get core limit");
            if (sizeof(rlim_t) == sizeof(long long))
            {
                printf("Hard core limit is: %lld\nsoft core limit is: %lld\n", (long long)core_lim.rlim_max, (long long)core_lim.rlim_cur);
            }
            else
            {
                printf("Hard core limit is: %ld\nsoft core limit is: %ld\n", (long)core_lim.rlim_max, (long)core_lim.rlim_cur);
            }
            break;
        case 'C':
            new_core_size = atol(optarg);
            new_rlim.rlim_cur = new_core_size;
            if (setrlimit(RLIMIT_CORE, &new_rlim) == -1) {
                perror("Error when changing the core-file size limit");
                continue;
            }
            if (sizeof(rlim_t) == sizeof(long long))
            {
                printf("The size of the core-file has been successfully changed to %lld bytes\n", (long long)new_rlim.rlim_cur);
            }
            else
            {
                printf("The size of the core-file has been successfully changed to %ld bytes\n", (long)new_rlim.rlim_cur);
            }
            break;
        case 'd':
            cwd = getcwd(NULL, 256);
            if (cwd == NULL) {
                perror("Error getting the current working directory");
                continue;
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
                continue;
            }
            printf("Environment variable successfully set or changed: %s\n", new_env_var);
            break;
        default:
            fprintf(stderr, "Usage: %s %s\n", argv[0], options);
            continue;
        }
    }

    return 0;
}
