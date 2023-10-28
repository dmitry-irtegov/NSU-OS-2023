#include <unistd.h>
#include <sys/termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define BELL write(1, "\a", 1);
#define BACKSPACE write(1, "\b \b", 3);

struct termios tty, savetty;

void reset_terminal() {
    tcsetattr(0, TCSANOW, &savetty);
}

int main() {
    if (!isatty(0)) {
        fprintf(stderr, "no stdin\n");
        exit(1);
    }

    if (tcgetattr(0, &tty) == -1) {
        perror("failed to get attributes");
        exit(1);
    }
    
    savetty = tty;
    atexit(reset_terminal);

    tty.c_lflag &= ~(ECHO | ICANON);
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &tty) == -1) {
        perror("failed to set attributes");
        exit(1);
    }

    char c;
    char line[100 + 1];
    int pos = 0;

    while (read(0, &c, 1) == 1) {
        if (c == CEOF && pos == 0) {
            break;
        }

        if (c == tty.c_cc[VERASE]) {
            if (pos > 0) {
                BACKSPACE;
                --pos;
            }
            
            continue;
        }

        if (c == tty.c_cc[VKILL]) {
            while (pos > 0) {
                BACKSPACE;
                --pos;
            }

            continue;
        }

        if (c == tty.c_cc[VWERASE]) {
            while (pos > 0 && isspace(line[pos - 1])) {
                BACKSPACE;
                pos--;
            }

            while (pos > 0 && !isspace(line[pos - 1])) {
                BACKSPACE;
                pos--;
            }

            continue;
        }

        if (c == '\n') {
            write(1, &c, 1);
            pos = 0;
            continue;
        }

        if (!isprint(c)) {
            BELL;
            continue;
        }

        if (pos == 40) {
            pos = 0;
            write(1, "\n", 1);
            continue;
        }

        if (isprint(c)) {
            write(1, &c, 1);
            line[pos++] = c;
            continue;
        }


    }

    return 0;
}