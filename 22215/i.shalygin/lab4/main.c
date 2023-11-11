#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 1024
typedef struct List {
    char* arr;
    struct List* next;
}List;
int main()
{
    printf("getting lines\n:");
    char current_line[MAX_LENGTH];
    List* head;
    int is_first = 1;
    while (fgets(current_line, MAX_LENGTH, stdin))
    {
        if (current_line[0] == '.')
            break;
        List* element = (List*)malloc(sizeof(List*));
        List* t = head;
        element->arr = (char*)malloc((strlen(current_line)+1) * sizeof(char));
        strcpy(element->arr,current_line);
        if (!is_first)
        {
            while (t->next != NULL)
                t = t->next;
            t->next = element;
        }
        else{
            head = element;
            is_first = 0;
        }
            
    }
    printf("Printing lines\n");
    List* prev;
    while (head)
    {
        printf("%s",head->arr);
	free(head->arr);
	prev = head;
        head = head->next;
	free(prev);
    }
    return 0;
}