#define _FILE_OFFSET_BITS 64
#define START_SIZE 4
#define WAIT_TIME 5
#define BUFFER_SIZE 10
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

typedef struct table
{
    off_t* arr;
    size_t capacity;
    size_t line_count;
} Table;


int allarm_calls = 0;

void table_construct(Table* table, int fd)
{
    table->arr = malloc(START_SIZE * sizeof(off_t));
    table->capacity = START_SIZE;
    table->line_count = 0;
}

void free_table(Table* table)
{
    free(table->arr);
    table->arr = NULL;
}

void print_file(int fd)
{
    char symbols[BUFSIZ];
    lseek(fd, 0, SEEK_SET);
    int readen = BUFSIZ;
    while ( readen == BUFSIZ)
    {
        readen = read(fd, &symbols, BUFSIZ);
        write(STDOUT_FILENO, &symbols, readen);
    }
}

void alarm_handler(int num_sig)
{
    signal(SIGALRM, alarm_handler);
    allarm_calls = 1;
}

void insert(Table* table, size_t index, off_t offset)
{
    if (table->capacity < index + 1)
    {
        table->arr = realloc(table->arr, 2 * (index + 1) * sizeof(off_t));
        table->capacity = 2 * (index + 1);
    }
    table->arr[index] = offset;
}

void create_symbols_table(Table* table, int fd)
{
    off_t line_offset = 0;
    size_t line = 0;
    char symbols[BUFSIZ];
    lseek(fd, 0, SEEK_SET);
    int readen = BUFSIZ;
    int prev_readen;
    while (readen > 0)
    {
        prev_readen = readen;
        readen = read(fd, &symbols, BUFSIZ);
        int count = 0;
        char* tmp = symbols;
        while (*tmp != '\0' && count < readen)
        {
            if (*tmp == '\n')
            {
                insert(table, line, ++line_offset);
                table->line_count++;
                line++;
                ++tmp;
                ++count;
                continue;
            }
            ++tmp;
            ++count;
            ++line_offset;
        }
    }
    if(symbols[prev_readen - 1] != '\n')
    {
        insert(table, line, ++line_offset);
        table->line_count++;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Not enough argument\n");
        return 1;
    }
    char* file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    if (fd == -1)
    {
        perror("File open error");
        return 1;
    }
    Table table;
    lseek(fd, 0, SEEK_SET);

    int bytesRead;
    table_construct(&table, fd);
    create_symbols_table(&table, fd);
    signal (SIGALRM, alarm_handler);
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0)
    {
        perror("Failed to set buffer");
        return 1;
    }
    while (1)
    {
        printf("Enter the line number: \n");
        alarm (WAIT_TIME);
        int number_line;
        int end = 0;
        if((end = scanf("%d", &number_line)) == 0)
        {
            number_line = -1;
            fprintf(stderr, "incorrect\n");
            break;
        }
        alarm (0);
        if(allarm_calls)
        {
            printf("\nTime Out!File contents:\n");
            print_file(fd);
            break;
        }
        if(end == -1)
        {
            fprintf(stderr, "incorrect scanf read\n");
            break;
        }
        if ((size_t)number_line > table.line_count || number_line < 0)
        {
            printf("Incorrect line number\n");
            continue;
        }
        --number_line;
        if (number_line == -1)
        {
            printf("File contains %zu lines\n", table.line_count);
            break;
        }
        size_t line_lenght = 0;
        if(number_line != 0)
        {
            line_lenght = table.arr[number_line] - table.arr[number_line - 1];
            lseek(fd, table.arr[number_line - 1], SEEK_SET);
        }
        else
        {
            line_lenght = table.arr[number_line];
            lseek(fd, 0, SEEK_SET);
        }
        char* buf = calloc(line_lenght, 1);
        read(fd, buf, line_lenght - 1);
        printf("%s\n", buf);
        free(buf);
    }
    free_table(&table);
    close(fd);
    return 0;
}
