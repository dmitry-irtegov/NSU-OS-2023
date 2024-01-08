#include <unistd.h>
#include <wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "More arguments expected \n");
        return -1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Error fork \n");
        return -1;
    }

    if (pid == 0)
    {
        if (execvp(argv[1], argv + 1) == -1)
        {
            perror("Error execvp \n");
	}
	return -1;
    }
    else
    {
        int status;
        wait(&status);

        if (WIFEXITED(status))
        {
	    printf("Child process (pid: %d) finished with exit code %d \n", pid, WEXITSTATUS(status));
	}

    }
    return 0;
}
