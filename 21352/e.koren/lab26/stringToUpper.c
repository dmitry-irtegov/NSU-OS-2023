#include <stdio.h>

#define STRLEN 31

int main(){
    char outputText[STRLEN] = "juSt an oUTput Text Sample 123\n";
    FILE *pipe = popen("./toUpper", "w");
    if (pipe == NULL){
        perror("failed to create pipe");
        return 1;
    }
    fwrite(outputText, sizeof(char), STRLEN, pipe);
    pclose(pipe);
    return 0;
}
