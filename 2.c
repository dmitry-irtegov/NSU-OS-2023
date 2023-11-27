#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

int main()
{
    time_t now;
    struct tm *sp;
    
    if(putenv("TZ=America/Los_Angeles")) {
        perror("Failed to set timezone");
    }

    sp = localtime(&now);
    printf("date: %d/%d/%02d\n",
           sp -> tm_mon + 1,
           sp -> tm_mday,
           sp -> tm_year);
    printf("time: %d:%d:%d\n",
           sp -> tm_hour,
           sp -> tm_min,
           sp -> tm_sec);
    return 0;
}