#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void try_open_file() {
  errno = 0;
  FILE* file = fopen("file", "r");
  if (file == NULL) {
    perror("Error opening file");
  }
  fclose(file);
}

int main() {
  printf("real user id: %d\neffectiv user id: %d\n", getuid(), geteuid());
  try_open_file();

  setuid(getuid());

  printf("real user id: %d\neffectiv user id: %d\n", getuid(), geteuid());
  try_open_file();
  return 0;
}