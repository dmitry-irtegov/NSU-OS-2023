#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int main() {
  errno = 0;
  time_t t = time(NULL);
  if (t == -1) {
    perror("time error");
  } else {
    putenv("TZ=PDT7");
    printf("California: %s", asctime(localtime(&t)));
  }

  return 0;
}