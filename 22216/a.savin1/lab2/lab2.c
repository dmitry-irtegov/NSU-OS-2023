#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

int main(){
    time_t now;
    struct tm *sp;

    putenv("TZ=US/Pacific");
    
    (void) time (&now);

    printf("%s", ctime( &now ) );

    sp = localtime(&now);
    printf("%d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year, sp->tm_hour,
        sp->tm_min, tzname[sp->tm_isdst]);
    return 0;
}
