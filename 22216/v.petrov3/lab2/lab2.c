#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int main() {
  time_t now;
  struct tm *sp;
  (void)time(&now);
  printf("%s", ctime(&now));

  if (putenv("TZ=America/Los_Angeles")) {
    perror("puting value of the time zone in the environment");
    exit(0);
  }
  
  printf("%s", ctime(&now));
  return 0;
}
