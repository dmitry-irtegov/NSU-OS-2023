#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    time_t now;
    if(time(&now) == -1) {
        perror("time()");
        exit(EXIT_FAILURE);
    }
    char buffer[26];
    if (putenv("TZ=America/Los_Angeles") < 0) {
        perror("putenv()");
        exit(EXIT_FAILURE);
    }
    if (ctime_r(&now, buffer, 26) == NULL) {
        perror("ctime_r()");
        exit(EXIT_FAILURE);
    }
    if (printf("%s", buffer) < 0) {
        perror("printf()");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);  
}