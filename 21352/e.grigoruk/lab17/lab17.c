#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 40

struct termios tty, savetty;

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savetty);
}

int main() {
    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "There is no stdin\n");
        exit(1);
    }

    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("Error getting attributes");
        exit(1);
    }

    savetty = tty;
    tty.c_lflag &= ~(ECHO | ICANON);
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("Error setting attributes");
        exit(1);
    }

    atexit(reset_terminal);

    char line[MAX_LINE_LENGTH + 1] = "";
    int pos = 0;
    char c;

    while (1) {
        c = getchar();

        if (c == CEOF && pos == 0) {
            break;
        }
        else if (c == '\n') {
            putchar(c);
            fflush(stdout);
            pos = 0;
        }
        else if (c == tty.c_cc[VERASE]) {
            if (pos > 0) {
                printf("\b \b");
                fflush(stdout);
                pos--;
            }
        }
        else if (c == tty.c_cc[VKILL]) {
            while (pos > 0) {
                printf("\b \b");
                fflush(stdout);
                pos--;
            }
        }
        else if (c == tty.c_cc[VWERASE]) {
            while (pos > 0 && line[pos - 1] == ' ') {
                printf("\b \b");
                pos--;
            }
            while (pos > 0 && line[pos - 1] != ' ') {
                printf("\b \b");
                pos--;
            }
            printf("\x1b[K");
            fflush(stdout);
        }
        else if (c >= 32 && c <= 126) {
            if (pos < MAX_LINE_LENGTH) {
                putchar(c);
                fflush(stdout);
                line[pos++] = c;
            }
        }
        else {
            putchar('\a');
            fflush(stdout);
        }
        if (pos == MAX_LINE_LENGTH) {
            printf("\n");
            pos = 0;
            fflush(stdout);
        }
    }
    return 0;
}
