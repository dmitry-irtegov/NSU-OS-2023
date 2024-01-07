#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


void printUID() {
    printf("Real user ID: %d\n", getuid());
    printf("Effective user ID: %d\n", geteuid());
}

void TryToOpenFile(char *fileName) {
    FILE *file;
    file = fopen(fileName, "r");

    if (file == NULL) {
	perror("Error: Can't open file.\n");
    }
    else {
	printf("File has been openned successfully!\n");

	if (fclose(file) == EOF) {
	    perror("Error: Can't close file.");
	}
    }
}

int main(int argc, char *argv[]) {
    printUID();
    TryToOpenFile(argv[1]);

    int setNewUID = seteuid(getuid());

    if (setNewUID == -1) {
	perror("Error: Failed to set new user ID.\n");
    }

    printUID();
    TryToOpenFile(argv[1]);

    return 0;
}
