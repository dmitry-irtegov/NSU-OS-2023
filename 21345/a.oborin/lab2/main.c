#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char* tzname[];

int main()
{
    time_t now;
    putenv("TZ=PST8PDT");
    now = time(NULL);
    printf("%s", ctime(&now));

    struct tm* sp = localtime(&now);
    printf("%d/%d/%d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday, 1900 + sp->tm_year,
        sp->tm_hour, sp->tm_min, tzname[sp->tm_isdst]);
    return 0;
}

