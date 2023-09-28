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

static void help();
static void handle_arg(const int argument);
static void handle_h();
static void handle_i();
static void handle_s();
static void handle_p();
static void handle_u();
static void handle_U();
static void handle_c();
static void handle_C();
static void handle_d();
static void handle_v();
static void handle_V();

int main(int argc, char *argv[])
{
   	char options[] = "hispuU:cC:dvV:";
	
	if (argc < 2)
	{
		help();
		return 0;
	}
    
	int argument;
	opterr = 0;
	
	while ((argument = getopt(argc, argv, options)) != EOF)
	{
		handle_arg(argument);
	}

	return 0;
}

static void help()
{
	printf("-i - print real and effective user and group ID`s\n"
		"-s - make current process leader of the group\n"
		"-p - print process, parent-process and group ID`s\n"
		"-u - print current ulimit\n"
		"-Unew_ulimit - set ulimit to new_ulimit\n"
		"-c - print core file size\n"
		"-Csize - set core file size to size\n"
		"-d - print current directory\n"
		"-v - print env variables\n"
		"-Vname=value - change env variable or create the new one if it does not exist yet\n\n");
}

static void handle_arg(const int argument)
{
	switch (argument)
	{
		case 'h': 
		{
			handle_h();
			break;
		}
		
		case 'i': 
		{
			handle_i();
			break;
		}

		case 's': 
		{
			handle_s();
			break;
		}

		case 'p':
		{
			handle_p();
			break;
		}

		case 'u': 
		{
			handle_u();
			break;
		}

		case 'U': 
		{
			handle_U();
			break;
		}

		case 'c': 
		{
			handle_c();
			break;
		}

		case 'C': 
		{ 
			handle_C();
			break;
		}

		case 'd': 
		{
			handle_d();
			break;
		}

		case 'v': 
		{
			handle_v();
			break;
		}

		case 'V': 
		{
			handle_V();
			break;
		}

		case '?':
		{
			printf("Invalid option: %c\n\n", optopt);
		}
	}
}

static rlim_t ortarg_to_rlim_t(const char* optarg)
{
	if (sizeof(long long) == sizeof(rlim_t))
	{
		return strtoll(optarg, NULL, 10); 
	}
	
	else 
	{
		return strtol(optarg, NULL, 10); 
	}
}

static void handle_h()
{
	help();
}

static void handle_i()
{
	printf("Real user id: %d\n", getuid());
	printf("Effective user id: %d\n", geteuid());
	printf("Real group id: %d\n", getgid());
	printf("Effective group id: %d\n\n", getegid());
}

static void handle_s()
{
	if (setpgid(0, 0) != 0)
	{
		perror("Unable to set process as leader of the group");
	}
}

static void handle_p()
{
	printf("Proccess ID: %d\n", getpid());
	printf("Parent ID: %d\n", getppid());
	printf("Group ID: %d\n\n", getpgid(0));
}

static void handle_u()
{
	struct rlimit u_limit;
	
   	if (getrlimit(RLIMIT_FSIZE, &u_limit) != 0)
	{
		perror("Unable to get ulimit");
		return;
	}
    
	if (u_limit.rlim_cur == RLIM_INFINITY) 
	{
        printf("ulimit soft: unlimited\n");
    }
	else 
	{
        printf("ulimit soft: %ll\n", (long long)u_limit.rlim_cur);
    }
	
	if (u_limit.rlim_max == RLIM_INFINITY) 
	{
        printf("ulimit hard: unlimited\n\n");
    }
	else 
	{
        printf("ulimit hard: %ll\n\n", (long long)u_limit.rlim_max);
    }
}

static void handle_U()
{
	errno = 0;
	rlim_t new_soft_limit = ortarg_to_rlim_t(optarg);
	if (errno != 0)
	{
		perror("Can not convert -U argument");
		return;
	}
	
	struct rlimit new_u_limit;
	if (getrlimit(RLIMIT_FSIZE, &new_u_limit) != 0)
	{
		perror("Unable to get current ulimit");
		return;
	}
	
	new_u_limit.rlim_cur = new_soft_limit;

	if (setrlimit(RLIMIT_FSIZE, &new_u_limit) != 0)
	{
		perror("Unable to change ulimit");
	}
}

static void handle_c()
{
	struct rlimit core_limit;
    if (getrlimit(RLIMIT_CORE, &core_limit) != 0)
	{
		perror("Unable to get core limit");
    }

	if (core_limit.rlim_cur == RLIM_INFINITY) 
	{
        printf("core limit soft: unlimited\n");
    }
	else 
	{
        printf("core limit soft: %ll\n", (long long)core_limit.rlim_cur);
    }	
	
	if (core_limit.rlim_max == RLIM_INFINITY) 
	{
       	printf("core limit hard: unlimited\n\n");
    }
	else 
	{
        printf("core limit hard: %ll\n\n", (long long)core_limit.rlim_max);
    }
}

static void handle_C()
{
	errno = 0;
	rlim_t new_soft_limit = ortarg_to_rlim_t(optarg);
	if (errno != 0)
	{
		perror("Can not convert -C argument");
		return;
	}
	
	struct rlimit new_core_limit;
	if (getrlimit(RLIMIT_CORE, &new_core_limit) != 0)
	{
		perror("Unable to get current core limit");
		return;
	}
	
	new_core_limit.rlim_cur = new_soft_limit;

	if (setrlimit(RLIMIT_CORE, &new_core_limit) != 0)
	{
		perror("Unable to change core limit");
	}
}

static void handle_d()
{
	char buf[PATH_MAX];
    if (getcwd(buf, PATH_MAX) == NULL)
	{
		perror("Can not get current working directory");
		return;
	}
	
    printf("Current working directory: %s\n\n", buf);
}

static void handle_v()
{
	for (char** env = environ; *env; ++env)
	{
       		printf("%s\n", *env);
    }
	printf("\n");
}

static void handle_V()
{
    if (putenv(optarg) != 0)
	{
		perror("Unable to putenv");
    }
}

