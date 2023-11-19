#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char *str;
    struct Node *next;
} Node;

typedef struct {
    Node *first;
    Node *last;
} List;

List initList() {
    List newList;
    newList.first = NULL;
    newList.last = NULL;
    return newList;
}

void addNode(List *list, char *string, size_t len) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->str = (char *) malloc(len + 1);
    strcpy(newNode->str, string);

    if (list->first == NULL) {
        list->first = newNode;
        list->last = newNode;
    } else {
        list->last->next = newNode;
        list->last = newNode;
    }
}

void freeList(List *list) {
    Node *nextNode;
    for (Node *curNode = list->first; curNode != NULL; curNode = nextNode) {
        nextNode = curNode->next;
        free(curNode->str);
        free(curNode);
    }
}

int main() {
    List strList = initList();

    char *buffer = (char *) malloc(BUFSIZ);
    size_t bufLen = BUFSIZ;

    while (getline(&buffer, &bufLen, stdin) != -1) {
        size_t strLen = strlen(buffer);

        if (strLen == 0)
            break;

        if (buffer[strLen - 1] == '\n') {
            buffer[strLen - 1] = '\0';
            strLen--;
        }

        if (buffer[0] == '.')
            break;

        addNode(&strList, buffer, strLen);
    }

    for (Node *curNode = strList.first; curNode != NULL; curNode = curNode->next) {
        printf("%s\n", curNode->str);
    }

    free(buffer);
    freeList(&strList);
    return 0;
}
