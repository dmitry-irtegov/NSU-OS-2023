#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "No file\n");
        exit(0);
    }

    int fd;

    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        perror("Cant open file");
        exit(1);
    }

    unsigned long long readedLinesNumber = 0;
    unsigned long long linesCounter = 2;
    size_t* linesShifts = (size_t*)malloc(linesCounter * sizeof(size_t));
    size_t* linesSizes = (size_t*)malloc(linesCounter * sizeof(size_t));
    struct stat fileInfo = { 0 };
    linesShifts[0] = 0;
    
    if (fstat(fd, (struct stat*)&fileInfo) == -1)
    {
        perror("Cant read info about file");
        exit(1);
    }
    
    char* filePtr;
    if ((filePtr = mmap(0, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        perror("Cant map file");
        exit(1);
    }
    
    char symbol;
    size_t lineShift = 0;
    size_t lineSize = 0;
    unsigned long long currentLinesCounter = 0; 
    for (size_t symbolCounter = 0; symbolCounter < fileInfo.st_size; symbolCounter++)
    {
        symbol = filePtr[symbolCounter];
        lineSize++;
        lineShift++;
        if (symbol == '\n')
        {
            linesSizes[currentLinesCounter] = lineSize - 1;
            currentLinesCounter++;
            readedLinesNumber++;
            if (currentLinesCounter == linesCounter)
            {
                linesCounter = linesCounter * 2;
                linesShifts = (size_t*)realloc(linesShifts, linesCounter * sizeof(size_t));
                linesSizes = (size_t*)realloc(linesSizes, linesCounter * sizeof(size_t));
                if (linesShifts == NULL || linesSizes == NULL) 
                {
                    perror("Cant rellocate memory");
                    exit(1);
                }
            }
            linesShifts[currentLinesCounter] = lineShift;
            lineSize = 0;
        }
    }
    unsigned long long lineNumber = 0;
    struct pollfd checkInput;
    memset(&checkInput, 0, sizeof(struct pollfd));
    checkInput.fd = STDIN_FILENO;
    checkInput.events = POLLIN;
    int timeout = 0;
    int isEnd = 0;
    while (1)
    {
        printf("Enter line number\n");
        timeout = poll(&checkInput, 1, 5000);
        if(timeout == 0)
        {
            unsigned long long linesCounter = 0;
	    size_t sizeOfFile = 0;
	    while(linesCounter <= readedLinesNumber)
            {
                sizeOfFile = sizeOfFile + linesSizes[linesCounter];
		linesCounter++;
            }
            write(1, filePtr, sizeOfFile + linesCounter);
	    break;

        }
        if ((isEnd = scanf("%ld", &lineNumber)) == 0)
        {
            fprintf(stderr, "Wrong input\n");
            scanf("%*[^\n]");
	    continue;
        }
        
	if(isEnd == -1)
	{
            break;
        }

        if (lineNumber == 0)
        {
            break;
        }

        if (lineNumber > readedLinesNumber)
        {
            fprintf(stderr, "Your number is too large\n");
            continue;
        }
        write(1, filePtr + linesShifts[lineNumber - 1], linesSizes[lineNumber - 1] + 1);
    }
    exit(0);
}
