#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    struct flock lock;
    int fd;
    char command[FILENAME_MAX];

    fd = open(argv[1], O_RDWR);
    if (fd == -1)
    {
        perror("Can't open file");
        return 1;
    }

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0; //
    lock.l_len = 0; //

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        if ((errno == EACCES) || (errno == EAGAIN))
        {
            printf("Try arain later, file is busy\n");
            close(fd);
            return 0;
        }
        perror("Can't lock file");
        close(fd);
        return 1;
    }

    sprintf(command, "vim %s\n", argv[1]);

    if (system(command) == -1) {
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Can't unlock file");
        }
        close(fd);
        return 1;
    }
    return 0;
    //рассказать про блокировки. виды и взаимодействия 
}