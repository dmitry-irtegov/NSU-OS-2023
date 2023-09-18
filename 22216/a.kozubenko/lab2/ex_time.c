#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

main()
{
    if (setenv("TZ", "PST8PDT", 1) == -1)
        perror("11: TZ change error");

    time_t now;
    struct tm *sp;

    (void) time( &now );

    printf("%s", asctime( localtime(&now) ) );

    sp = localtime(&now);

    printf("%d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year + 1900, sp->tm_hour,
        sp->tm_min, tzname[sp->tm_isdst]);
    
    exit(0);
}