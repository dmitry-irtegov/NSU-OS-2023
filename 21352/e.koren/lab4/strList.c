#include<stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
        char* data;
        struct Node* nextNode;
}Node;

Node* createNode() {
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
                perror("malloc error");
                return NULL;
        }
        node->data = NULL;
        node->nextNode = NULL;
        return node;
}

Node* setNodeData(char* str) {
        Node* node = createNode();

        node->data = (char*)calloc((strlen(str) + 1),sizeof(char));
        if (node->data == NULL) {
                perror("calloc error");
                free(node);
                return NULL;
        }

        memcpy(node->data, str, (strlen(str) + 1) * sizeof(char));

        return node;
}

void freeNode(Node* node) {
        if (node == NULL) {
                return;
        }
        node->nextNode = NULL;
        free(node->data);
        free(node);
}

void freeList(Node* head) {
        if (head == NULL) {
                return;
        }
        if (head->nextNode == NULL) {
                freeNode(head);
                return;
        }
        freeList(head->nextNode);
        freeNode(head);
        return;
}

void printNodeData(Node* node) {
        if (node == NULL && node->data==NULL) return;
        printf("%s", node->data);
        return;
}

void printList(Node* head) {
        if (head == NULL) { return; }
        while (head != NULL) {
                printNodeData(head);
                head = head->nextNode;
        }
        return;
}

#define BUFFERSIZE 256

int main() {
        char buffer[BUFFERSIZE];

        Node* head = createNode();
        if (head == NULL) {
                perror("couldnt malloc");
                return 1;
        }
        Node* currentNode = head;
        printf("enter lines. to finish input insert '.'\n");

        while (fgets(buffer, BUFFERSIZE, stdin) != NULL) {
                if (buffer[0] == '.') {
                        break;
                }
                currentNode->nextNode = setNodeData(buffer);
                if (currentNode->nextNode == NULL) {
                        if (head == NULL) {
                                perror("couldnt malloc");
                                freeList(head);
                                return 1;
                        }
                }
                currentNode = currentNode->nextNode;
        }
        printf("output\n");
        printList(head->nextNode);

        freeList(head);
        return 0;
}
