#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void get_user_info(){
    printf("Real user-id: %d\n", getuid());
    printf("Effective user-id: %d\n", geteuid());
}

void open_file(const char* s){
    FILE* file;
    if((file = fopen(s, "r"))==NULL){
        perror("Cannot open file\n");
    }
    else{
        fclose(file);
    }
}

int main(int argc, char** argv){
    get_user_info();
    open_file(argv[1]);

    setuid(getuid());
    get_user_info();
    open_file(argv[1]);

}