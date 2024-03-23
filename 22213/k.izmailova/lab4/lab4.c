#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
        struct Node* next;
        char* string;
}Node;

Node* init_root()
{
    Node* root = (Node*)malloc(sizeof(Node));
    root->next = NULL;
    root->string = NULL;
    return root;
}

Node* add_string(char string[])
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL)
    {
        printf("Not enough memory for new node\n");
        return NULL;
    }
    new_node->next = NULL;
    new_node->string = (char*)malloc(sizeof(char) * (strlen(string) + 1));
    if(new_node->string == NULL)
    {
        printf("Not enough memory for new string\n");
        return NULL;
    }
    strcpy(new_node->string, string);
    return new_node;
}


void free_nodes(Node* root)
{
    while(root->next != NULL)
    {
        Node* temp_node = root;
        root = root->next;
        if (temp_node->string != NULL)
        {
            free(temp_node->string);
        }
        free(temp_node);
    }
}

int main()
{
    char in_string[1024];
    Node* root = NULL;
    root = init_root();
    Node* current_node = root;
    while(1)
    {
        if(fgets(in_string, 1024, stdin) == NULL)
        {
            printf("Error while input.\n");
            break;
        }
        if(in_string[0] == '.')
        {
            break;
        }
        current_node->next = add_string(in_string);
        if(current_node->next == NULL)
        {
            printf("Error while adding new string\n");
            break;
        }
        current_node = current_node->next;
    }
    current_node = root;
    printf("\nResult:\n");
    while(current_node->next != NULL)
    {
        printf("%s", current_node->next->string);
        current_node = current_node->next;
    }
    free_nodes(root);
}
