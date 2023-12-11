#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
    time_t now;

    putenv("TZ=America/Los_Angeles");

    (void) time (&now);

    printf("%s", ctime( &now ) );

    return 0;
}
