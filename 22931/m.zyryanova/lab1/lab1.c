#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <limits.h>

extern char** environ;

int main(int argc, char* argv[]) {
    //char options[] = "id";

    int c = 0;
    while((c = getopt(argc, argv, "ispuU:cC:dvV:")) != EOF)
    {
        //printf("%c", c);
        switch(c)
        {
            case 'i':
                printf("Real user id: %d\n", getuid());
                printf("Effective user id: %d\n", geteuid());
                printf("Real group id: %d\n", getgid());
                printf("Effective group id: %d\n", getegid());
                break;

            case 's':
                setpgid(0, getgid());
                printf("The process became a leader of the group\n");
                break;

            case 'p':
                printf("Process id: %d\n", getpid());
                printf("Parent-process id: %d\n", getppid());
                printf("Group of processes id: %d\n", getpgid(0));
                break;

            case 'u':
                printf("ulimit: %ld\n", ulimit(UL_GETFSIZE));
                break;

            case 'U':
                ulimit(UL_SETFSIZE, atol(optarg));
                printf("Your limit changed to %s\n", optarg);
                break;

            case 'c':
                printf("Max size of possible core-file: %d\n", RLIMIT_CORE);
                break;

            case 'C':
                struct rlimit new_limit = {atol(optarg), atol(optarg)};
                setrlimit(RLIMIT_CORE, &new_limit);
                break;

            case 'd':
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
                break;

            case 'v':
                for(int i = 0; environ[i]!=0; i++)
                {
                    printf("%s\n", environ[i]);
                }
                break;

            case 'V':
                putenv(optarg);
                break;
        }
    }

    return 0;
}
