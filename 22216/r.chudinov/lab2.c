#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern char *tzname[];

int main(){
        time_t now;
        struct tm*sp;
        if (putenv("TZ=America/Los_Angeles") != 0){
                perror("Environment variable setting error");
                exit(-1);
        }


        if (time(&now) == -1){
                printf("Time getting error.\n");
                exit(-1);
        }

        sp = localtime(&now);
        printf("%02d/%02d/%02d %02d:%02d:%02d %s\n",
                sp->tm_mday, sp->tm_mon + 1,
                sp->tm_year + 1900, sp->tm_hour,
                sp->tm_min, sp->tm_sec,
                tzname[sp->tm_isdst]);

        return 0;
}
