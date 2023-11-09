#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
int main(){
    char ch;
    struct termios tty, savtty;
    int fd;
    fd = open("/dev/tty", O_RDONLY);
    tcgetattr(fd, &tty);
    if (isatty(fileno(stdout)) == 0) {
     fprintf(stderr,"stdout not terminal\n");
     exit(1);
    }
    savtty = tty;
    tty.c_lflag &= ~(ICANON);
    tty.c_cc[VMIN] = 1;     /* MIN */
    tcsetattr(fd, TCSAFLUSH, &tty);
    
    printf("How are you?\n");
    read(fd, &ch, 1);
    tcsetattr(fd, TCSAFLUSH, &savtty);
    return 0;
  }