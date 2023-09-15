#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    //setenv(const char *name, const char *value, int overwrite);
    setenv("TZ", "America/Los_Angeles", 1);

    //time_t time(time_t *tloc);
    time_t current_time;
    time(&current_time);

    //char *ctime(const time_t *timep);
    printf("time in California is : %s\n", ctime(&current_time));

    return 0;
}

