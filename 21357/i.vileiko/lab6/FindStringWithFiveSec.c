#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct Line{
    off_t offset;
    off_t length;
} Line;

typedef struct Vector{
    Line* array;
    int cnt;
    int cap;
} Vector;

void vector_init(Vector* vec) {
    vec->array = malloc(sizeof(Line));
    vec->cnt = 0;
    vec->cap = 1;
}

void vector_push(Vector* vec, Line element) {
    if (vec->cnt == vec->cap) {
        vec->cap *= 2;
        vec->array = realloc(vec->array, vec->cap * sizeof(Line));

        if (vec->array == NULL) {
            perror("Realloc has failed");
            exit(1);
        }
    }
    vec->array[vec->cnt++] = element;
}

void vector_destroy(Vector* vec) {
    free(vec->array);
    vec->array = NULL;
    vec->cnt = 0;
    vec->cap = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    Vector* table = malloc(sizeof(Vector));
    vector_init(table);

    int fd_terminal = open("/dev/tty", O_RDONLY | O_NDELAY);
    if (fd_terminal == -1) {
        perror("/dev/tty");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Couldnt open the file");
        exit(1);
    }

    lseek(fd, 0LL, SEEK_SET);

    char c;
    off_t offset = 0;
    off_t length = 0;
    off_t max_length = 0;

    while (read(fd, &c, 1) == 1) {
        if (c == '\n') {
            Line current = {offset, length};
            vector_push(table, current);

            if (length > max_length)
                max_length = length;

            offset += length + 1;
            length = 0;
        }
        else {
            length++;
        }
    }

    if (length > 0) {
        Line current = {offset, length};
        vector_push(table, current);
    }

    Line line_info;
    char *str = calloc(max_length + 2, sizeof(char));
    char *buf = malloc(BUFSIZ * sizeof(char));
    int i, n;

    while (1) {
        printf("Enter the line number within 5 seconds: \n");
        sleep(5);

        if ((i = read(fd_terminal, buf, BUFSIZ)) < 1) {
            printf("No line number was provided. Printing whole file:\n");
            lseek(fd, 0LL, SEEK_SET);

            while((i = read(fd, buf, BUFSIZ)) > 0) {
                write(1, buf, i);
            }
            break;
        }

        buf[i] = 0;
        n = atoi(buf);

        if (n == 0) {
            break;
        }

        if (table->cnt < n) {
            printf("The file contains only %d lines\n", table->cnt);
            continue;
        }

        line_info = table->array[n - 1];

        lseek(fd, line_info.offset, SEEK_SET);
        read(fd, str, line_info.length * sizeof(char));
        str[line_info.length] = 0;

        printf("%s\n", str);
    }

    close(fd);
    close(fd_terminal);
    free(str);
    vector_destroy(table);

    return 0;
}
