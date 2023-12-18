#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_BUFFER_SIZE 1024
#define TIME_OUT 5

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numFiles = argc - 1;
    FILE **files = (FILE **)malloc(numFiles * sizeof(FILE *));
    char **fileNames = (char **)malloc(numFiles * sizeof(char *));
    fd_set fdSet;
    int maxFd = 0;
    char buffer[MAX_BUFFER_SIZE];
    int n;

    // Открываем файлы для чтения
    for (int i = 0; i < numFiles; i++) {
        fileNames[i] = argv[i+1];
        files[i] = fopen(fileNames[i], "r");
        if (files[i] == NULL) {
            fprintf(stderr, "Error opening file: %s\n", fileNames[i]);
            exit(EXIT_FAILURE);
        }
        // Находим максимальный файловый дескриптор
        if (fileno(files[i]) > maxFd) {
            maxFd = fileno(files[i]);
        }
    }

    // Читаем из файлов по очереди
    while (1) {
        FD_ZERO(&fdSet);
        // Добавляем файловые дескрипторы в набор
        for (int i = 0; i < numFiles; i++) {
            if (files[i] != NULL) {
                FD_SET(fileno(files[i]), &fdSet);
            }
        }

        // Ждем событий в наборе файловых дескрипторов
        int result = select(maxFd + 1, &fdSet, NULL, NULL, NULL);
        if (result < 0) {
            perror("Error in select");
            break;
        } else if (result == 0) {
            // Тайм-аут истек, берем следующий файл
            fprintf(stderr, "Time-out expired\n");
            break;
        }

        // Читаем из файлов, у которых есть данные
        for (int i = 0; i < numFiles; i++) {
            if (files[i] != NULL && FD_ISSET(fileno(files[i]), &fdSet)) {
                // Читаем строку из файла
                if (fgets(buffer, sizeof(buffer), files[i]) != NULL) {
                    printf("%s: %s", fileNames[i], buffer);
                } else {
                    // Достигнут конец файла, закрываем файл
                    fclose(files[i]);
                    files[i] = NULL;
                }
            }
        }

        // Проверяем, закончились ли все файлы
        int finished = 1;
        for (int i = 0; i < numFiles; i++) {
            if (files[i] != NULL) {
                finished = 0;
                break;
            }
        }
        if (finished) {
            break;
        }
    }

    // Закрываем все файлы
    for (int i = 0; i < numFiles; i++) {
        if (files[i] != NULL) {
            fclose(files[i]);
        }
    }

    free(files);
    free(fileNames);

    return 0;
}
