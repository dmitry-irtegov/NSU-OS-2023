#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct Node {
    char* data;
    struct Node* nextNode;
} Node;

Node* createNode() {
    Node* node = (Node*)malloc(1 * sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->nextNode = NULL;
    node->data = NULL;
    return node;
}

Node* fillNodeData(char* line) {
    Node* node = createNode();
    if (node == NULL) {
        return NULL;
    }

    node->data = (char*)malloc((strlen(line) + 1));
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, line, (strlen(line) + 1));

    node->nextNode = NULL;
    return node;
}

void freeNode(Node* node) {
    node->nextNode = NULL;
    free(node->data);
    free(node);
}

void freeList(Node* head) {
    if (head == NULL) {
        return;
    }

    Node* nextNode = head->nextNode;
    Node* tmp = NULL;
    while (nextNode != NULL) {
        tmp = nextNode->nextNode;
        freeNode(nextNode);
        nextNode = tmp;
    }

    freeNode(head);
}

void printList(Node* head) {
    if (head == NULL) { 
        return; 
    }
    Node* node;
    for (node = head->nextNode; node != NULL; node = node->nextNode) {
        printf("%s", node->data);
    }
}

#define BUFFERSIZE 255

int main() {
    char buffer[BUFFERSIZE];

    Node* head = createNode();
    if (head == NULL) {
        perror("Malloc error\n");
        return -1;
    }
    Node* currentNode = head;

    printf("Enter your strings. To finish enter '.'\n");

    while (fgets(buffer, BUFFERSIZE, stdin) != NULL) {

        if (buffer[0] == '.') {
            break;
        }

        currentNode->nextNode = fillNodeData(buffer);
        if (currentNode->nextNode == NULL) {
            perror("Malloc error\n");
            freeList(head);
            return -1;
        }

        currentNode = currentNode->nextNode;
    }

    printList(head);
    freeList(head);

    return 0;
}
