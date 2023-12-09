#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    //opening file descriptor
    int terminalDescriptor = open("/dev/tty", O_RDWR);
    if (terminalDescriptor == -1 || !isatty(terminalDescriptor)) { //the terminal wasn't opened or the descriptor describes a non-terminal device
        perror("Terminal wasn't opened");
        exit(1);
    }

    struct termios modifiedTerminal, savedTerminal;
    if (tcgetattr(terminalDescriptor, &modifiedTerminal) == -1) {
        perror("Couldn't get terminal's attributes");
        exit(1);
    }

    savedTerminal = modifiedTerminal; //saving the attributes of the current terminal

    modifiedTerminal.c_lflag &= ~ICANON; //set non-canonical mode
    modifiedTerminal.c_cc[VMIN] = 1; //set the minimum number of characters that must be available for reading to 1
    modifiedTerminal.c_cc[VTIME] = 0;
    if (tcsetattr(terminalDescriptor, TCSANOW, &modifiedTerminal) == -1) { 
        perror("Couldn't set terminal's changed attributes");
        exit(1);
    }

    char answer[2] = "", string question = "Do you like sleeping, chilling and drinking beer? ";
    write(terminalDescriptor, question, strlen(question));
    read(terminalDescriptor, answer, 1);
    while (answer[0] != 'y' && answer[0] != 'Y') {
        printf("\nWrong answer! Go out and think more... \n");
        write(terminalDescriptor, question, strlen(question));
        read(terminalDescriptor, answer, 1);
    }
    printf("\nRight answer!\n");

    if (tcsetattr(terminalDescriptor, TCSAFLUSH, &savedTerminal) == -1) { 
        perror("Couldn't restore terminal's original attributes");
    }
    return 0;
}