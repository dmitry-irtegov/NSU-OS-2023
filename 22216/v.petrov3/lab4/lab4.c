#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN 2048

typedef struct Node {
    struct Node* next;
    char *string;
} Node;

Node* init_elem(int len, char *array){
    Node* new_elem = (Node*)malloc(sizeof(Node));
    new_elem->next = NULL;
    new_elem->string = (char*)malloc(sizeof(char) * (len + 1));
    memcpy(new_elem->string, array, (len + 1));
    return new_elem;
}

Node* push_elem(Node *elem, Node* head){
    Node* current_elem = head;
    if (head == NULL) head = elem;
    else {
        while(current_elem->next != NULL){
            current_elem = current_elem->next;
        }
        current_elem->next = elem;
    }
    return head;
}

void input(Node **(main_head)){
    char array[MIN];
    Node *head = NULL;
    char *out;
    while (1) {
        out = fgets(array, MIN, stdin);
        if (out == NULL) {
            perror("fgets failed");
            exit(1);
        }
        if (array[0] == '.'){ 
            break; 
        }
        int len = strlen(out) + 1;
        head = push_elem(init_elem(len, array), head);
    }
    *(main_head) = head;
}

void output(Node *head){
    Node *current_elem = head;
    while (current_elem->next != NULL) {
        printf("%s\n", current_elem->string);
        current_elem = current_elem->next;
    }
    if (current_elem != NULL) {printf("%s", current_elem->string);}
}

void free_func(Node *head){
    Node *current_elem = head;
    while (current_elem->next != NULL) {
        Node *tmp = current_elem->next;
        free(current_elem->string);
        free(current_elem);
        current_elem = tmp;
    }
    if (current_elem != NULL) {free(current_elem->string); free(current_elem);}
}

int main() {
    Node *head;
    input(&head);
    output(head);
    free_func(head);
    return 0;
}
