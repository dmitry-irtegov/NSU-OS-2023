#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No filename was provided\n");
        return 1;
    }

    char* filename = argv[1];

    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("failed to open the file");
        return 1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        if (errno == EAGAIN) {
            printf("%s file read is locked\n", argv[1]);
            return 0;
        }
        perror("failed to lock the file");
        return 1;
    }
    

    size_t needed_mem = snprintf(NULL, 0, "nano %s", filename) + sizeof('\0');

    char *command = malloc(needed_mem);
    if (command == NULL){
        perror("failed to allocate memory");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("failed to unlock the file");
            return 1;
        }
        if (close(fd) == -1){
            perror("failed to close the file");
            return 1;
        }
        return 1;
    }

    snprintf(command, needed_mem, "nano %s", filename);

    if (system(command) == -1){
        perror("failed to execute a command");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("failed to unlock the file");
            return 1;
        }
        if (close(fd) == -1){
            perror("failed to close the file");
            return 1;
        }
        return 1;
    }

    free(command);

    lock.l_type = F_UNLCK;


    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("failed to unlock the file");
        return 1;
    }
    if (close(fd) == -1){
        perror("failed to close the file");
        return 1;
    }

    return 0;
}