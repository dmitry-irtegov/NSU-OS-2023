#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define START_SIZE 50

typedef struct {
    off_t *offsets;
    size_t *lengths;
    int size;
    int capacity;
} DynamArr;


void freedynamArr(DynamArr *dynamArr) {
    free(dynamArr->offsets);
    free(dynamArr->lengths);
    dynamArr->size = 0;
    dynamArr->capacity = 0;
}

int initdynamArr(DynamArr *dynamArr) {
    dynamArr->offsets = (off_t *) malloc(START_SIZE * sizeof(off_t));
    if (dynamArr->offsets == NULL) {
        perror("Error malloc ");
        return -1;
    }
    dynamArr->lengths = (size_t *) malloc(START_SIZE * sizeof(size_t));
    if (dynamArr->offsets == NULL) {
        perror("Error malloc ");
        return -1;
    }
    dynamArr->size = 0;
    dynamArr->capacity = START_SIZE;
    return 0;
}

int add(DynamArr *dynamArr, off_t offset, size_t length) {
    if (dynamArr->size == dynamArr->capacity) {
        dynamArr->capacity *= 2;

        off_t* newo;
        newo = (off_t *) realloc(dynamArr->offsets, dynamArr->capacity * sizeof(off_t));
        if (newo == NULL) {
            perror("Error realloc ");
            return -1;
        }
        dynamArr->offsets = newo;

        size_t* newl;
        newl = (size_t *) realloc(dynamArr->lengths, dynamArr->capacity * sizeof(size_t));
        if (newl == NULL) {
            perror("Error realloc ");
            return -1;
        }
        dynamArr->lengths = newl;
    }
    dynamArr->offsets[dynamArr->size] = offset;
    dynamArr->lengths[dynamArr->size++] = length;
    return 0;
}



int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error input of programm. Programm usage example is: \n%s <filename>\n", argv[0]);
        return -1;
    }


    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    DynamArr table;
    if(initdynamArr(&table) < 0) {
        exit(EXIT_FAILURE);
    } 

    char buffer[BUFSIZ];
    ssize_t bytes_read;
    off_t prev_offset = 0;
    off_t curr_offset = 0;


    while ((bytes_read = read(fd, buffer, BUFSIZ)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            curr_offset++;
            if (buffer[i] == '\n') {
                if(add(&table, prev_offset, curr_offset - prev_offset) < 0) {
                    exit(EXIT_FAILURE);
                }
                prev_offset = curr_offset;
            }
        }
    }

    if(add(&table, prev_offset, curr_offset - prev_offset) < 0) {
        exit(EXIT_FAILURE);
    }

    if (bytes_read == -1) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }

    if (table.size == 0) {
        printf("File is empty. Nothing to do. End of the programm\n");
        return 0;
    }

    while(1) {
        
        int findLine = -1;
        printf("Input number of target line: ");
        if ((scanf("%d", &findLine) != 1) || findLine <= 0 || findLine > table.size) {
            fprintf(stderr, "Incorrect number of line. line should be a number from 1 to %d.\n", table.size);
            __fpurge(stdin);
            continue;
        }
        findLine--;
        off_t curr_offset = table.offsets[findLine];

        lseek(fd, curr_offset, SEEK_SET);

        char buffer_str[BUFSIZ];

        off_t tmp = curr_offset;
        off_t target_offset = table.offsets[findLine] + table.lengths[findLine];

        size_t read_size = (BUFSIZ > table.lengths[findLine] ? table.lengths[findLine] : BUFSIZ);

        while ((bytes_read = read(fd, buffer_str, read_size)) > 0) {
            tmp += bytes_read;
            if (tmp < target_offset) {
                if (fwrite(buffer_str, sizeof(char), bytes_read, stdout) != bytes_read){
                    fprintf(stderr, "Error to write in stdout");
                    exit(EXIT_FAILURE);
                }
                curr_offset = tmp;
            }
            else {
                if (fwrite(buffer_str, sizeof(char), target_offset - curr_offset, stdout) != target_offset - curr_offset){
                    fprintf(stderr, "Error to write in stdout");
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }

        if (++findLine == table.size) {
            if (fwrite("\n", sizeof(char), 1, stdout) != 1) {
                fprintf(stderr, "Error to write in stdout");
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1) {
            perror("Error reading file");
            exit(EXIT_FAILURE);
        }
    }
    close(fd);
    freedynamArr(&table);
    return 0;
}
