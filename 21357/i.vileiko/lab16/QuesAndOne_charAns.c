#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


struct termios tty, savetty;

void reset_terminal() {
    tcsetattr(0, TCSANOW, &savetty);
}

int main() {
    if (!isatty(0)) {
        fprintf(stderr, "No stdin!\n");
        exit(1);
    }

    if (tcgetattr(0, &tty) == -1) {
        perror("Failed to get attributes");
        exit(1);
    }

    savetty = tty;
    atexit(reset_terminal);

    tty.c_lflag &= ~(ECHO | ICANON);
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &tty) == -1) {
        perror("Failed to set attributes");
        exit(1);
    }

    printf("yes(y) or no(n)?\n");

    char c;
    read(0, &c, 1);

    if (c == 'y') {
        printf("YES!\n");
    } else if (c == 'n') {
        printf("NO!\n");
    } else {
        printf("You entered: %c\n", c);
    }

    return 0;
}
