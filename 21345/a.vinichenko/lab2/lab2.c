#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    time_t current_time;
    struct tm *local_time;

    setenv("TZ", "America/Los_Angeles", 1);

    time(&current_time);

    local_time = localtime(&current_time);

    printf("Time in California: %s\n", asctime(local_time));

    unsetenv("TZ");

    return 0;
}
