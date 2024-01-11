#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main() {
  struct termios tty;
  struct termios old_term;

  tcgetattr(0, &old_term);
  tty = old_term;

  tty.c_lflag &= (~ICANON);
  tty.c_cc[VTIME] = 0;
  tty.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, &tty);
  char x;
  read(0, &x, 1);

  tcsetattr(0, TCSANOW, &old_term);

  return 0;
}