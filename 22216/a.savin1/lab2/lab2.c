#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    time_t now;

    (void) time( &now );
    now -= 3600 * 14;

    printf("%s", ctime( &now ) );

    exit(0);
}
