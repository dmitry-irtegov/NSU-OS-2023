#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char* tzname[];

int main()
{
    time_t now;
    (void)time(&now);
    struct tm* sp;
    putenv("TZ=America/Los_Angeles");
    sp = localtime(&now);
<<<<<<< HEAD
	
=======

>>>>>>> ad8baad (lab_2)
    printf("California time: %d/%d/%02d %d:%02d %s\n",
         sp->tm_mon + 1, sp->tm_mday,
         sp->tm_year, sp->tm_hour,
         sp->tm_min, tzname[sp->tm_isdst]);
<<<<<<< HEAD
		 
=======
>>>>>>> ad8baad (lab_2)
    exit(0);
}
