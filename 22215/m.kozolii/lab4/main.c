#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* next;
    char* data;
} Node;

Node* initializeNode(char* value) {
    Node* node = malloc(sizeof(Node));
    int length = strlen(value) + 1;
    node->data = malloc(length * sizeof(char));
    strcpy(node->data, value);
    node->next = NULL;
    return node;
}

Node* addNode(Node* list, char* value) {
    if (list != NULL) {
        Node* tmp = list;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = initializeNode(value);
        return list;
    } else {
        return initializeNode(value);
    }
}

void printList(Node* list) {
    while (list != NULL) {
        printf("%s", list->data);
        list = list->next;
    }
}

void freeList(Node* list) {
    while (list != NULL) {
        Node* tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}

int main() {
    char buffer[BUFSIZ];
    Node* list = NULL;

    while (fgets(buffer, BUFSIZ, stdin) != NULL &&
           buffer[0] != '.' && buffer[strlen(buffer) - 1] == '\n') {
        list = addNode(list, buffer);
    }

    if (buffer[strlen(buffer) - 1] != '\n' && buffer[0] != '.') {
        perror("String size exceeds BUFSIZ");
    }

    printList(list);
    freeList(list);

    return 0;
}
