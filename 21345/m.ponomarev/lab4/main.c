#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Storage {
    char *line; // string
    struct Storage *next; // pointer to the next line
} Storage;

Storage* createStorageNode(char *input) {
    // Calculate the length of the input string
    size_t len = strlen(input);

    // Allocate memory for a new node and string
    Storage *newLine = (Storage *)malloc(sizeof(Storage));

    if (newLine == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    newLine->line = (char *)malloc(len + 1); // +1 for the null character
    if (newLine->line == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Copy the input string into the node
    strncpy(newLine->line, input, len);
    newLine->line[len] = '\0'; // add the null character

    newLine->next = NULL; // set the pointer to the next node as NULL

    return newLine;
}

void insertNode(Storage **head, Storage **cur, Storage *newLine) {
    // If the list is empty, set the new node as the head
    if (*head == NULL) {
        *head = newLine;
        *cur = newLine;
    }
    else {
        (*cur)->next = newLine;
        *cur = newLine;
    }
}


void printStorage(Storage *head) {
    // Print all the lines from the list
    printf("Input lines:\n");
    Storage *cur = head;
    while (cur != NULL) {
        printf("%s", cur->line);
        cur = cur->next;
            }
        }

        void freeStorage(Storage *head) {
            // Free the memory
            Storage *cur = head;
            while (cur != NULL) {
                Storage *tmp = cur;
                cur = cur->next;
                free(tmp->line);
                free(tmp);
            }
        }

        int main() {
            Storage *head = NULL; // start
            Storage *cur = NULL; // current line

            char input[1024];

            while (1) {
                printf("Enter a string or . : ");
                fgets(input, sizeof(input), stdin); // get a string from stdin and stor>

                if (input[0] == '.' && input[1] == '\n') {
                    break; // exit the loop if .\n is entered
                }

                Storage *newLine = createStorageNode(input);
                insertNode(&head, &cur, newLine);
            }
            printStorage(head);
            freeStorage(head);
            return 0;
        }

