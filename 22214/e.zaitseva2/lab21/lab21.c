#define _XOPEN_SOURCE 500
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void sigint_handler(int signum) {
  signal(signum, sigint_handler);
  count++;
}

void sigquit_handler(int signum) {
  printf("\ncount: %d\n", count);
  exit(0);
}

int main() {
  signal(SIGINT, sigint_handler);

  signal(SIGQUIT, sigquit_handler);

  while (1) {
  }

  return 0;
}