 #include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define TIME_OUT 5

typedef struct Node {
    int fd;
    char* filename;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    Node* tail;
} List;

List* CreateList() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

Node* CreateNode(const char* str, int fd) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->filename = (char*)malloc(strlen(str) + 1);
    if (new_node->filename == NULL) {
        return NULL;
    }
    strcpy(new_node->filename, str);
    new_node->fd = fd;
    new_node->next = NULL;
    return new_node;
}

void insert_list(List* list, const char* str, int fd) {
    if (str == NULL || list == NULL) {
        return;
    }
    Node* new_node = CreateNode(str, fd);
    if (new_node == NULL) {
        return;
    }
    if (list->head == NULL) {
        list->head = new_node;
    }
    else {
        list->tail->next = new_node;
    }
    list->tail = new_node;
}

void destroy_head(List* list) {
    if (list == NULL) {
        return;
    }
    Node* deleted_node = list->head;
    list->head = list->head->next;
    free(deleted_node->filename);
    free(deleted_node);
}

int flag_alarm = 0;

void alarm_handler(int signum) {
    signal(SIGALRM, alarm_handler);
    flag_alarm = 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Filename no passed. \n");
        return 1;
    }

    List* list = CreateList();
    if (list == NULL) {
        perror("Memory error");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }
        insert_list(list, argv[i], fd);
    }

    signal(SIGALRM, alarm_handler);

    char buf[1024];
    int bytes_read;

    while (1) {
        alarm(TIME_OUT);
        bytes_read = read(list->head->fd, buf, sizeof(buf));
        alarm(0);
        if (bytes_read > 0) {
            write(STDOUT_FILENO, buf, bytes_read);
        }
        else if (bytes_read == 0) {
            close(list->head->fd);
            printf("File %s is close, the end of the file. \n", list->head->filename);
            destroy_head(list);
            if (list->head == NULL) {
                break;
            }
        }
        else if (bytes_read < 0 && flag_alarm) {
            flag_alarm = 0;
            close(list->head->fd);
            printf("File %s is close, the waiting time has expired. \n", list->head->filename);
            destroy_head(list);
            if (list->head == NULL) {
                break;
            }
        }
        else {
            perror("Reading file error");
            return 1;
        }
    }
    return 0;
}
