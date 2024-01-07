#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

void print_line(int fd, off_t* table_offsets, int line_counter) {
    lseek(fd, table_offsets[line_counter], SEEK_SET);
    size_t line_length = (size_t)(table_offsets[line_counter + 1] - table_offsets[line_counter] - 1);
    char buf[10];
    int readed = 1;
    while (readed != 0 && line_length > 0) {
        if (line_length < sizeof(buf)) {
            readed = read(fd, buf, line_length);
        }
        else {
            readed = read(fd, buf, sizeof(buf) - 1);
        }
        if (readed == -1) {
            perror("Error reading file");
            free(table_offsets);
            exit(1);
        }
        line_length -= readed;
        buf[readed] = '\0';
        printf("%s", buf);
    }
    printf("\n");
}

void prinf_file(int fd, off_t* table_offsets, int line_count) {
    for (int line_counter = 0; line_counter < line_count; line_counter++) {
        print_line(fd, table_offsets, line_counter);
    }
}

void fill_table(off_t** table_offsets, int* line_count, off_t* start_size, int fd, off_t* shift_line, off_t shift) {
    (*table_offsets)[(*line_count) + 1] = shift + 1;
    *shift_line = 0;
    (*line_count) = (*line_count) + 1;
    if (*line_count + 1 == *start_size) {
        *start_size = (*start_size) * 2;
        (*table_offsets) = (off_t*)realloc((*table_offsets), (*start_size) * sizeof(off_t));
        if (*table_offsets == NULL) {
            perror("Memory allocation error");
            exit(1);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Filename not passed\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("File opening error");
        return 1;
    }

    off_t start_size = 5;
    off_t* table_offsets = (off_t*)malloc(start_size * sizeof(off_t));
    if (table_offsets == NULL) {
        perror("Memory allocation error");
        close(fd);
        return 1;
    }

    int end_file = 0;
    int line_num = 0;
    off_t shift = 0;
    off_t shift_line = 0;
    char input[5];
    char current_char = '0';
    table_offsets[0] = 0;
    int readen_chars = read(fd, input, sizeof(input));
    while (readen_chars != 0) {
        if (readen_chars == -1) {
            perror("File reading error");
            close(fd);
            free(table_offsets);
            return 1;
        }
        for (int char_counter = 0; char_counter < readen_chars; char_counter++) {
            current_char = input[char_counter];
            if (current_char == '\n') {
                fill_table(&table_offsets, &line_num, &start_size, fd, &shift_line, shift);
            }
            else {
                shift_line++;
            }
            shift++;
        }
        readen_chars = read(fd, input, sizeof(input));
        if (readen_chars < sizeof(input)) {
            end_file = 1;
        }
    }
    if (end_file) {
        fill_table(&table_offsets, &line_num, &start_size, fd, &shift_line, shift);
    }

    int continue_input = 1;

    while (continue_input) {

        printf("Enter the line number (0 for exit):\n");

        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ready == -1) {
            perror("Error waiting for input");
            close(fd);
            free(table_offsets);
            return 1;
        }
        if (ready == 0) {
            printf("The waiting time has expired. File:\n");
            prinf_file(fd, table_offsets, line_num);
            continue_input = 0;
        }
        else {
            char buffer[100];
            scanf("%s", buffer);
            int line_number = atoi(buffer);

            if (line_number == 0 && buffer[0] != '0') {
                printf("Invalid line number.\n");
                continue;
            }

            buffer[strcspn(buffer, "\n")] = '\0';
            char str[100];
            sprintf(str, "%d", line_number);

            if (strlen(str) != strlen(buffer)) {
                printf("Invalid line number.\n");
                continue;
            }
            if (line_number == 0) {
                continue_input = 0;
            }
            if (line_number >= line_num) {
                printf("Invalid line number.\n");
            }
            else {
                print_line(fd, table_offsets, line_number - 1);
            }
        }
    }
    free(table_offsets);
    close(fd);
    return 0;
}

