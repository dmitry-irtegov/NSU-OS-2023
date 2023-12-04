#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int fileDescriptor = 0;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file_name\n", argv[0]);
        return 1;
    }

    if ((fileDescriptor = open(argv[1], O_RDONLY)) == -1) {
        perror("Open error");
        return 1;
    }

    size_t lines_capasity = 8;
    off_t* lengths = (off_t*)malloc(lines_capasity * sizeof(off_t));
    if (lengths == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    off_t* offsets = (off_t*)malloc(lines_capasity * sizeof(off_t));
    if (offsets == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    char str_buffer[255];
    off_t offset = 0;
    off_t lineIdx = 1;
    off_t lineCount = 0;
    off_t bytesRead = 1;
    off_t cur_line_length = 0;

    while (bytesRead > 0) {
        bytesRead = read(fileDescriptor, str_buffer, sizeof(str_buffer));
        if (bytesRead == -1) {
            perror("failed to read");
            return 1;
        }

        for (off_t i = 0; i < bytesRead; i++) {
            cur_line_length++;
            offset++;
            if (str_buffer[i] == '\n') {
                lengths[lineIdx] = cur_line_length;
                offsets[lineIdx] = offset - cur_line_length;
                cur_line_length = 0;
                lineIdx++;
                if (lineIdx >= lines_capasity) {
                    lines_capasity = lines_capasity * 2;
                    offsets = (off_t*)realloc(offsets, lines_capasity * sizeof(off_t));
                    if (offsets == NULL) {
                        perror("Memory reallocation failed");
                        return 1;
                    }
                    lengths = (off_t*)realloc(lengths, lines_capasity * sizeof(off_t));
                    if (lengths == NULL) {
                        perror("Memory reallocation failed");
                        return 1;
                    }
                }
            }
        }
    }
    lineCount = lineIdx;

    printf("Enter line number from 1 to %d. Enter 0 to exit\n", lineCount - 1);

    size_t lineToPrint = 1;

    while (true) {
        printf("line number: ");

        if (lineToPrint == 0) {
            break;
        }

        if (scanf("%u", &lineToPrint) != 1) {
            fflush(stdin);
            fprintf(stderr, "Invalid input\n");
            continue;
        }

        if (lineToPrint < 0 || lineToPrint > lineCount - 1) {
            printf("Line number is an integer number from 1 to %d\n", lineCount - 1);
            continue;
        }

        if (lineToPrint == 0) {
            break;
        }

        if (lseek(fileDescriptor, offsets[lineToPrint], SEEK_SET) == -1) {
            perror("failed to change pointer position in file");
            break;
        }

        off_t line_len = lengths[lineToPrint];
        off_t line_remain = line_len - 1;
        off_t was_readed = 1;
        char buffer[255];
        
        while (was_readed != 0 && line_remain > 0) {
            if (line_remain < sizeof(buffer)) {
                was_readed = read(fileDescriptor, buffer, line_remain);
            }
            else {
                was_readed = read(fileDescriptor, buffer, sizeof(buffer) - 1);
            }
            if (was_readed == -1) {
                perror("read error");
                return 1;
            }
            line_remain -= was_readed;
            buffer[was_readed] = '\0';
            printf("%s", buffer);
        }
        printf("\n");

    }
    if (close(fileDescriptor) != 0) {
        perror("error occurred closing file");
        return 1;
    }
    return 0;
}
