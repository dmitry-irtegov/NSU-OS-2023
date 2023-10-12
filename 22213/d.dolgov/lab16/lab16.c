#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("%s", "Hello! How many people are there on earth in billions now?\n\nYour answer: ");
    static struct termios oldTerminal, newTerminal;
    tcgetattr(STDIN_FILENO, &oldTerminal); // get settings of the current terminal
    newTerminal = oldTerminal; // copy settings
    newTerminal.c_lflag &= ~(ICANON);
    newTerminal.c_cc[VMIN] = 1;
    newTerminal.c_cc[VTIME] = 0;
    // TCSANOW used to change attributes immediately.
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal); // applying to STDIN
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
    exit(0);
}
