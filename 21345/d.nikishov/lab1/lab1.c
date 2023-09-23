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
    int print_ids_flag = 0;
    int set_leader_flag = 0;
    int print_process_info_flag = 0;
    int print_ulimit_flag = 0;
    int change_ulimit_flag = 0;
    long new_ulimit = 0;
    int print_core_size_flag = 0;
    int change_core_size_flag = 0;
    long new_core_size = 0;
    int print_working_directory_flag = 0;
    int print_environment_flag = 0;
    char* new_env_var = NULL;

    if (argc == 1) {
        fprintf(stderr, "Mistake: no options are specified. Usage: %s [options]\n", argv[0]);
        return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, options)) != EOF) {
        switch (opt) {
        case 'i':
            print_ids_flag = 1;
            break;
        case 's':
            set_leader_flag = 1;
            break;
        case 'p':
            print_process_info_flag = 1;
            break;
        case 'u':
            print_ulimit_flag = 1;
            break;
        case 'U':
            change_ulimit_flag = 1;
            new_ulimit = atol(optarg);
            break;
        case 'c':
            print_core_size_flag = 1;
            break;
        case 'C':
            change_core_size_flag = 1;
            new_core_size = atol(optarg);
            break;
        case 'd':
            print_working_directory_flag = 1;
            break;
        case 'v':
            print_environment_flag = 1;
            break;
        case 'V':
            new_env_var = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s %s\n", argv[0], options);
            exit(EXIT_FAILURE);
        }
    }

    if (print_ids_flag) {
        uid_t real_uid = getuid();
        uid_t efficie_uid = geteuid();
        gid_t real_gid = getgid();
        gid_t efficie_gid = getegid();
        printf("Real UID: %d\n", real_uid);
        printf("Effective UID: %d\n", efficie_uid);
        printf("Real GID: %d\n", real_gid);
        printf("Effective GID: %d\n", efficie_gid);
    }

    if (set_leader_flag) {
        pid_t pid = getpid();
        if (setpgid(pid, pid) == -1) {
            perror("Failed to set group");
            exit(EXIT_FAILURE);
        }
    }

    if (print_process_info_flag) {
        pid_t pid = getpid();
        pid_t ppid = getppid();
        pid_t pgid = getpgrp();
        printf("PID of the current process: %d\n", pid);
        printf("PPID of the parent process: %d\n", ppid);
        printf("PGID of the process group: %d\n", pgid);
    }

    if (print_ulimit_flag) {
        long ulimit_value = ulimit(UL_GETFSIZE);
        if (ulimit_value == -1) {
            perror("Error receiving ulimit");
            exit(EXIT_FAILURE);
        }
        printf("ulimit value: %ld\n", ulimit_value);
    }

    if (change_ulimit_flag) {
        if (new_ulimit < 0) {
            fprintf(stderr, "Error: Invalid value of the new ulimit\n");
            exit(EXIT_FAILURE);
        }
        if (ulimit(UL_SETFSIZE, new_ulimit) == -1) {
            perror("Error when changing ulimit");
            exit(EXIT_FAILURE);
        }
        printf("ulimit value successfully changed to %ld\n", new_ulimit);
    }

    if (print_core_size_flag) {
        struct rlimit core_lim;
        if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
            perror("failed to get core limit");
        printf("hard core limit is: %lu\nsoft core limit is: %lu\n", core_lim.rlim_max, core_lim.rlim_cur);
    }

    if (change_core_size_flag) {
        struct rlimit new_rlim;
        new_rlim.rlim_cur = new_core_size;
        new_rlim.rlim_max = new_core_size;
        if (setrlimit(RLIMIT_CORE, &new_rlim) == -1) {
            perror("Error when changing the core-file size limit");
            exit(EXIT_FAILURE);
        }
        printf("The size of the core-file has been successfully changed to %lld bytes\n", (long long)new_rlim.rlim_cur);
    }

    if (print_working_directory_flag) {
        long size = pathconf(".", _PC_PATH_MAX);
        char* buf = (char*)malloc((size_t)size);
        char* cwd = getcwd(buf, (size_t)size);
        if (cwd == NULL) {
            perror("Error getting the current working directory");
            exit(EXIT_FAILURE);
        }
        printf("Current working directory: %s\n", cwd);
        free(buf);
    }

    if (print_environment_flag) {
        for (char** env = environ; *env != NULL; env++) {
            printf("%s\n", *env);
        }
    }

    if (new_env_var != NULL) {
        if (putenv(new_env_var) != 0) {
            perror("Error setting environment variable");
            exit(EXIT_FAILURE);
        }
        printf("Environment variable successfully set or changed: %s\n", new_env_var);
    }

    return 0;
}