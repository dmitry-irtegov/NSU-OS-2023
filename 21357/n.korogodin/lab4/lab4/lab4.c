#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

Node* createNode(char* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data);
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, char* data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* previous = current;
        current = current->next;
        free(previous->data);
        free(previous);
    }
}

int main() {
    Node* head = NULL;
    char input[100];
    fgets(input, sizeof(input), stdin);
    while (input[0] != '.') {
        input[strlen(input) - 1] = '\0'; // remove newline character
        insertNode(&head, input);
        fgets(input, sizeof(input), stdin);
    }
    printList(head);
    freeList(head);
    return 0;
}
