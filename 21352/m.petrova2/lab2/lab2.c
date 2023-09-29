
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

 int  main()
{
     time_t seconds;
     struct tm *date_time;
     
     setenv("TZ", "America/Los_Angeles", 1);
     
     time(&seconds);
     
     date_time = localtime(&seconds);
     
     printf("Time in California: %s\n", asctime(date_time));
     
     unsetenv("TZ");
     
     return 0;
 }
