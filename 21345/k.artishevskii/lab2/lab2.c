#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

int main(int argc, char* argv[])
{
        putenv("TZ=America/Los_Angeles");
        time_t now;
        struct tm *sp;
        (void) time( &now );
        printf("%s", ctime( &now ) );
        sp = localtime(&now);
        printf("%d/%d/%02d %d:%02d %s\n",
                sp->tm_mon + 1, sp->tm_mday,
                sp->tm_year - 100, sp->tm_hour,
                sp->tm_min, tzname[sp->tm_isdst]);
<<<<<<< HEAD
        exit(0);
=======
        return 0;
>>>>>>> 1a2aa1ea9f88db71c77717241b09ef66f8eb257f
}