#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void fileWork(FILE* file){
    if (file == NULL){
        perror("Couldn't open file");
        exit(1);
    }
    else{
        fclose(file);
    }
}
void idPrint(){
    printf("User real id is %d\n", getuid());
    printf("User effective is %d\n", geteuid());
}
int main(int argc, char* argv[]){
    idPrint();
    FILE* file = fopen("1.txt", "w");
    fileWork(file);
    if (setuid(getuid()) == -1){
        perror("Couldn't change setuid");
        exit(1);
    }
    idPrint();
    file = fopen("1.txt", "w");
    fileWork(file);
    exit(0);
}
