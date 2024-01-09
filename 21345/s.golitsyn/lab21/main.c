#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static unsigned int bell_counter = 0;

void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        write(STDOUT_FILENO, "\a", 1);
        ++bell_counter;
    }
    else
    {
        if (signal == SIGQUIT)
        {
            char buffer[37];
            sprintf(buffer, "\nNumber of bell rings: %d\n", bell_counter);
            write(STDOUT_FILENO, buffer, strlen(buffer));
            exit(EXIT_SUCCESS);
        }
    }
}

void main()
{
    if (sigset(SIGINT, signal_handler) == SIG_ERR)
    {
        perror("sigset failure");
        exit(EXIT_FAILURE);
    }

    if (sigset(SIGQUIT, signal_handler) == SIG_ERR)
    {
        perror("sigset failure");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        pause();
    }
}
