#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 2046

typedef struct Node {
    char* string;
    struct Node* next;
} Node;

void print_list(Node* head) {
    printf("%s", head->string);
    if (head->next != 0) {
        print_list(head->next);
    }
}

Node* add_new_elem(Node* previous, char* value, int len) {
    Node* elem = (Node*)malloc(sizeof(Node));
    elem->string = (char*)malloc(len);
    strcpy(elem->string, value);
    elem->next = 0;
    previous->next = elem;
    return elem;
}

int main() {
    char* input_string = (char*)malloc(BUFSIZE);
    int allocate_to;

    fgets(input_string, BUFSIZE, stdin);
    allocate_to = strlen(input_string) + 1;
    Node* last = (Node*)malloc(sizeof(Node)), *first;
    first = last;
    last->string = malloc(allocate_to);

    if (input_string[0] != '.') {
        strcpy(last->string, input_string);
    } else {
        return 0;
    }

    while(1) {
       fgets(input_string, BUFSIZE, stdin);

        if (input_string[0] != '.') {
            allocate_to = strlen(input_string) + 1;
            Node* elem = add_new_elem(last, input_string, allocate_to);
            last = elem;
        } else {
            break;
        }
    }

    print_list(first);

    return 0;
}
