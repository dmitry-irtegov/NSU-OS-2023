#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFF_SIZE 32

int main(int argc, char** argv)
{
    int pipe_fd[2];
    
    int read_pipe_fd;
    int write_pipe_fd;
    pid_t pid;
    
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }
    
    read_pipe_fd = pipe_fd[0];
    write_pipe_fd = pipe_fd[1];
    
    pid = fork();
    
    if (pid == -1)
    {
        perror("pid failure");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0)
    {
        close(read_pipe_fd);
        char messageWr[BUFF_SIZE] = "SuPeR sTrInG";
        write(write_pipe_fd, messageWr, BUFF_SIZE);
        close(write_pipe_fd);
    }
    
    if (pid > 0)
    {
        close(write_pipe_fd);
        char messageRd[BUFF_SIZE];
        int len;
        while((len = read(read_pipe_fd, messageRd, BUFF_SIZE)) > 0)
        {
            for (int i = 0; i < len; i++){
                messageRd[i] = toupper(messageRd[i]);
            }
        }
        printf("%s", messageRd);
        close(read_pipe_fd);
    }
    return 0;
}
