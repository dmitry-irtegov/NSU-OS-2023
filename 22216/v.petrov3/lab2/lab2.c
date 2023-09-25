#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int main() {
  time_t now;
  struct tm *sp;
  (void)time(&now);

  printf("%s", ctime(&now));

  if (putenv("TZ=UTC+8")) {
    perror("puting value of the time zone in the environment");
    exit(0);
  }

  sp = localtime(&now);

  printf("%d/%d/%02d %d:%02d %s\n", sp->tm_mon + 1, sp->tm_mday, sp->tm_year,
         sp->tm_hour, sp->tm_min, tzname[sp->tm_isdst]);

  return 0;
}
