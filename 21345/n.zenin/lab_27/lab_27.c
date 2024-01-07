#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define RESULT_SIZE 64

char* get_command(char* command_name)
{
    long max_file_name_length = pathconf(".", _PC_NAME_MAX);
    char command_begin_name[9] = "grep ^$ ";
    char command_end_name[9] = " | wc -l";
    char* command = calloc(max_file_name_length + sizeof(command_begin_name) + sizeof(command_end_name), 1);
    strcat(command, command_begin_name);
    strcat(command, command_name);
    strcat(command, command_end_name);
    return command;
}

int main(int argc, char** argv)
{
    char result[RESULT_SIZE];
    if (argc < 2)
    {
        fprintf(stderr, "Not enought arguments\n");
        exit(1);
    }
    if (access(argv[1], F_OK) == -1)
    {
        perror("File not found");
        exit(1);
    }
    char* command = get_command(argv[1]);
    FILE* result_output_file = popen(command, "r");
    if (!result_output_file)
    {
        perror("Error popen");
        free(command);
        exit(1);
    }
    fgets(result, sizeof(result), result_output_file);

    int exit_status = pclose(result_output_file);
    if(exit_status != 0)
    {
        perror("Error pclose");
        free(command);
        exit(1);
    }
    printf("Empty string count: %s", result);
    free(command);
    return 0;
}
