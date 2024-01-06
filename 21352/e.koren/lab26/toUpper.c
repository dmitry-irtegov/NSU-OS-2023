#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define STRLEN 512

int main(){
    char inputString[STRLEN];
    int stringLength;
    while ((stringLength = read(STDIN_FILENO, inputString, STRLEN)) > 0){
        for (int i = 0; i < stringLength; i++){
            putchar(toupper(inputString[i]));
        }
    }

    return 0;
}
