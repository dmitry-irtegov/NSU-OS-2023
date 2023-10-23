#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>

extern int errno;
extern char** environ;
extern char* optarg;
extern int optopt;
extern int opterr;

rlim_t rlim_type_select(const char* optarg)
{
    if (sizeof(rlim_t) == sizeof(long))
    {
        return strtol(optarg, NULL, 10);
    }
    return strtoll(optarg, NULL, 10);
}

void print_rlimit(rlim_t* value)
{
    if (sizeof(rlim_t) == sizeof(long))
    {
        printf("%lu\n", *value);
        return;
    }
    printf("%llu\n", *value);
}

int main(int argc, char* argv[])
{
    char options[] = "ispuU:cC:dvV:";
    int c;
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            case 'i':
                printf("User ID %d\n",getuid());
                printf("Effective user ID %d\n",geteuid());
                printf("Group ID %d\n",getgid());
                printf("Effective group ID %d\n",getegid());
                break;
            case 's':{
                printf("[%d] Original process group id: %d\n", getpid(), getpgid(0));
                if (setpgid(0, 0) == -1) {
                    perror("Error setpgid");
                    break;
                }
                printf("[%d] New process group id: %d\n", getpid(), getpgid(0));
                }
                break;
            case 'p':{
                printf("Process ID %d\n",getpid());
                printf("Parent process ID %d\n",getppid());
                printf("ID the group of process %d\n",getpgid(0));
                break;
                }
            case 'u':{
                struct rlimit rlp;
                if (getrlimit(RLIMIT_FSIZE, &rlp) != 0) {
                    perror("Error ulimit\n");
                    break;
                } else {
                    printf("Ulimit soft value: ");
                    print_rlimit(&rlp.rlim_cur);
                    printf("Ulimit hard value: ");
                    print_rlimit(&rlp.rlim_max);
                }
                break;
                }
            case 'U':{
                struct rlimit rlp;
                if (getrlimit(RLIMIT_FSIZE, &rlp) != 0) {
                    perror("Error ulimit\n");
                    break;
                }
                rlim_t new_limit = rlim_type_select(optarg);
                if (new_limit == 0) {
                    perror("Invalid argument for -U\n");
                    break;
                }
                rlp.rlim_cur = new_limit;
                if(setrlimit(RLIMIT_FSIZE, &rlp) != 0){
                    perror("Error setrlimit\n");
                }else{
                    printf("Successfully changed limit param\n");
                }
                break;
            }
            case 'c': {
                struct rlimit core_limit;
                if (getrlimit(RLIMIT_CORE, &core_limit) != 0) {
                    perror("Error ulimit\n");
                    break;
                }
                printf("Soft core file limit is ");
                print_rlimit(&core_limit.rlim_cur);
                printf("Hard core file limit is ");
                print_rlimit(&core_limit.rlim_max);
                break;
            }
            case 'C': {
                struct rlimit core_limit;
                core_limit.rlim_cur = rlim_type_select(optarg);
                if (setrlimit(RLIMIT_CORE, &core_limit) != 0){
                    perror("Error modification core file limit\n");
                }
                break;
            }
            case 'd': {
                long max_file_size = pathconf("/", _PC_PATH_MAX);
                char *path_name = getcwd(NULL, max_file_size);
                if (path_name == NULL) {
                    perror("fail to get CWD");
                    continue;
                }
                printf("Current directory: %s\n", path_name);
                free(path_name);
                path_name = NULL;
                break;
            }
            case 'v': {
                char **ptr = environ;
                for (; *ptr != NULL; ptr++) {
                    printf("%s\n", *ptr);
                }
                break;
            }
            case 'V': {
                if (putenv(optarg) != 0) {
                    perror("Failed to set the environmental variable");
                }
                break;
            }
            case '?':
                printf("Invalid option is %c\n", optopt);
            }

        }
}
