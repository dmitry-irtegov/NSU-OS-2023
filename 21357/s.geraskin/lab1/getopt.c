#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>

extern char** environ;
extern char* optarg;

int main(int argc, char *argv[]){
    char options[] = "ispuU:cC:dvV:";
    int c;

    printf("argc equals %d\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF){
        switch (c){
            case 'i':;
                printf("Real user id: %d\n", getuid());
                printf("Effective user id: %d\n", geteuid());
                printf("Real group id: %d\n", getgid());
                printf("Effective group id %d\n", getegid());
                break;

            case 's':;
                setpgid(0, 0);
                break;

            case 'p':;
                printf("Process id: %d\n", getpid());
                printf("Parent process id: %d\n", getppid());
                printf("Process group id: %d\n", getpgrp());
                break;

            case 'u':;
                struct rlimit u_limit;
                if (getrlimit(RLIMIT_FSIZE, &u_limit) == 0) {
                    if (u_limit.rlim_cur == RLIM_INFINITY) {
                        printf("ulimit: unlimited\n");
                    }
                    else {
                        printf("ulimit: %ld\n", u_limit.rlim_cur);
                    }
                }
                else {
                    perror("cannot get ulimit");
                }
                break;

            case 'U':;
                errno = 0;
                char* u_arg = optarg;
                long new_U_limit = atol(optarg);
                if (errno != 0 || new_U_limit < 0) {
                    fprintf(stderr, "Invalid new ulimit value: %s\n", u_arg);
                }

                struct rlimit U_limit;
                if (getrlimit(RLIMIT_FSIZE, &U_limit) == 0) {
                    U_limit.rlim_cur = (rlim_t) new_U_limit;
                    if (setrlimit(RLIMIT_FSIZE, &U_limit) != 0) {
                        perror("cannor set ulimit");
                    }
                }
                else {
                    perror("cannor get ulimit");
                }
                break;

            case 'c':;
                struct rlimit c_limit;
                if (getrlimit(RLIMIT_CORE, &c_limit) == 0) {
                    printf("Core file size limit: %ld\n", c_limit.rlim_cur);
                }
                else {
                    perror("cannot get core limit");
                }
                break;

            case 'C':;
                errno = 0;
                char* c_arg = optarg;
                long new_C_limit = atol(c_arg);
                if (errno != 0 || new_C_limit < 0) {
                    fprintf(stderr, "Invalid new core limit value: %s\n", c_arg);

                }

                struct rlimit C_limit;
                if (getrlimit(RLIMIT_CORE, &C_limit) == 0) {
                    C_limit.rlim_cur = (rlim_t) new_C_limit;
                    if (setrlimit(RLIMIT_CORE, &C_limit) != 0) {
                        perror("cannot set core limit");
                    }
                }
                else {
                    perror("cannot get core limit");
                }
                break;

            case 'd':;
                char buf[512];
                char *dir = getcwd(buf, 512);
                if (dir) {
                    printf("%s\n", dir);
                }
                else {
                    perror("cannot print dirrectory");
                }
                break;

            case 'v':;
                for (char **env = environ; *env; ++env){
                    printf("%s\n", *env);
                }

            case 'V':;
                char* v_arg = optarg;
                if (putenv(v_arg) != 0){
                    perror("cannot putenv");
                }
            default:;
                perror("unknown option");
        }
    }
    return 0;
}
