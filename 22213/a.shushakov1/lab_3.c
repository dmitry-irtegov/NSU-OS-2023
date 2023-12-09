#include <stdio.h>
#include <unistd.h>
int main(int argc, char* argv[]){
    printf("User real id is %d\n", getuid());
    printf("User effective is %d\n", geteuid());
    FILE* file= fopen("1.txt","r");
    if (file == NULL){
        perror("Couldn't open file");
        exit(1);
    }
    else{
        fclose(file);
    }
    if (setuid(getuid()) == -1){
        perror("Couldn't change setuid");
        exit(1);
    }

    printf("User real id is %d\n", getuid());
    printf("User effective is %d\n", geteuid());
    file= fopen("file"  ,"r");
    if (file == NULL){
        perror("Couldn't open file");
        exit(1);
    }
    else{
        fclose(file);
    }
    exit(0);
}
