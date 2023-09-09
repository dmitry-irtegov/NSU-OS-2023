#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

int main(){
    time_t now;

    (void) time( &now );

    if (setenv("TZ", "America/Los_Angeles", 1) != 0)
        perror("An error has occured");

    
    printf("%s", ctime( &now ) );
    
    
    exit(0);
}