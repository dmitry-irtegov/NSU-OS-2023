#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>


typedef struct Line {
    off_t offset;
    off_t length;
} Line;

typedef struct Vector {
    Line* array;
    int count;
    int capacity;
} Vector;

void vector_init(Vector* vec) {
    vec->array = malloc(sizeof(Line));
    vec->count = 0;
    vec->capacity = 1;
}

void vector_pushBack(Vector* vec, Line elem) {
    if (vec->count == vec->capacity) {
        vec->capacity = vec->capacity * 2;
        vec->array = realloc(vec->array, vec->capacity * sizeof(Line));

        if (vec->array == NULL) {
            perror("Error: Realloc has not done!");

            exit(1);
        }
    }

    vec->array[vec->count++] = elem;
}

void vector_destroy(Vector* vec) {
    free(vec->array);
    vec->array = NULL;
    vec->count = vec->capacity = 0;
}

int fd;
Vector* table;
off_t size;
char* mapped;

void handler_alarm(int signum){

    printf("\n\n");
    write(1, mapped, size);

    exit(0);
}


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No filename was provided\n");
        return 1;
    }

    table = malloc(sizeof(Vector));
    vector_init(table);

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("Error: Couldnt open the file!");
        exit(1);
    }

    lseek(fd, 0LL, SEEK_SET);

    size = lseek(fd, 0, SEEK_END);

    mapped = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (mapped == MAP_FAILED) {
        exit(1);
    }

    char c;
    off_t offset = 0;
    off_t length = 0;
    off_t max_length = 0;

    for (int i = 0; i < size; i++) {
        c = mapped[i];

        if (c == '\n') {
            Line current = {offset, length};
            vector_pushBack(table, current);

            if (length > max_length) {
                max_length = length;
	    }

            offset += length + 1;
            length = 0;
        }
        else {
            length++;
        }
    }

    if (length > 0) {
        Line current = {offset, length};
        vector_pushBack(table, current);
    }

    Line line_info;
    int n;

    signal(SIGALRM, handler_alarm);

    while (1) {
        printf("Enter the line number within 5 seconds: \n");
        alarm(5);

        if (scanf("%d", &n) != 1) {
            perror("Error: Incorrect input!");
            exit(1);
        }

        alarm(0);

        if (n == 0) {
            break;
        }

        if (table->count < n) {
            printf("The file contains only %d lines\n", table->count);
            continue;
        }

        line_info = table->array[n - 1];

        write(1, mapped + line_info.offset, line_info.length);
        printf("\n");
    }


    close(fd);
    vector_destroy(table);

    exit(0);
}
