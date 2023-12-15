#include <stdio.h>

#define MAX_LEN 256

int main(int argv, char* argc[]){
    FILE *pipe = popen("./printInUpper", "w");
    if (pipe == NULL){
        perror("ERROR: pipe not create");
        return -1;
    }

    int LEN = 0;
    for(int i = 1; i < argv; ++i){
        for(int j = 0; argc[i][j] != '\0'; ++j){
            LEN = j;
        }
        fwrite(argc[i], sizeof(char), LEN + 1, pipe);
        char spliter[1] = " ";
        fwrite(spliter, sizeof(char), 1, pipe);
    }
    pclose(pipe);
    return 0;
}
