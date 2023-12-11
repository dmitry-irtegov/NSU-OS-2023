#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern char *tzname[];

int main(){
    time_t now;
    struct tm*sp;
    if (putenv("TZ=America/California") != 0){
        perror("Environment error");
        exit(-1);
    }

    if (time(&now) == -1){
        perror("Time getting error");
        exit(-2);
    }

    
    sp = localtime(&now);
    printf("%02d/%02d/%02d %02d:%02d:%02d %s\n",
           sp->tm_mday, sp->tm_mon + 1,
           sp->tm_year + 1900, sp->tm_hour,
           sp->tm_min, sp->tm_sec,
           tzname[sp->tm_isdst]);

    return 0;
}
