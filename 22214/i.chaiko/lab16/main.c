#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios terminal_attributes_old, terminal_attributes_new;

    if (tcgetattr(STDIN_FILENO, &terminal_attributes_old) != 0) {
        perror("terminal attributes error");
        exit(1);
    }

    terminal_attributes_new = terminal_attributes_old;
    terminal_attributes_new.c_lflag &= (~ICANON);
    terminal_attributes_new.c_cc[VTIME] = 0;
    terminal_attributes_new.c_cc[VMIN] = 1;
    //TCSADRAIN TCSAFLUSH
    if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal_attributes_new) != 0) {
        perror("terminal setup error");
        exit(1);
    }

    printf("?\n");
    char s;

    read(0, &s, 1);
    printf("\n");

    tcsetattr(STDIN_FILENO, TCSADRAIN, &terminal_attributes_old);

    return 0;
}
