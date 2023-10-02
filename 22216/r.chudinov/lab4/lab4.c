#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_OF_BUFFER 1024

typedef struct Node{
        char * string;
        struct Node * next;
} Node;

void cleanMemory(Node * startNode){
        Node * tmp;
        while(startNode != NULL){
                tmp = startNode;
                startNode = startNode->next;
                free(tmp->string);
                free(tmp);
        }
}

Node *initNode(char *str){
        Node * startNode = malloc(sizeof(Node));
        if (startNode == NULL){
                perror("Memory isn't allocated");
                return NULL;
        }
        startNode->string = malloc(strlen(str) + 1);
        if (startNode->string == NULL){
                perror("Memory for string isn't allocated");
                free(startNode);
                return NULL;
        }
        strcpy(startNode->string, str);
        startNode->next = NULL;
        return startNode;
}

Node *addToEnd(char *str, Node * startNode){
        if (startNode == NULL){
                startNode = initNode(str);
                if(startNode == NULL){
                        exit(-1);
                }
                return startNode;
        }

        Node * tmp = startNode;
        while(tmp->next != NULL){
                tmp = tmp->next;
        }
        tmp->next = initNode(str);
        if(tmp->next == NULL){
                cleanMemory(startNode);
                exit(-1);
        }
        return startNode;
}

int main() {
        char buffer[SIZE_OF_BUFFER + 1];
        Node * node = NULL;
        printf("Enter lines:\n");
        while(1){
                if (fgets(buffer, SIZE_OF_BUFFER + 1, stdin) == NULL){
                        break;
                }
                if (buffer[0] == '.'){
                        break;
                }
                node = addToEnd(buffer, node);
        }

        printf("Output:\n");
        Node * tmp;
        tmp = node;
        while(tmp != NULL){
                printf("%s",tmp->string);
                tmp = tmp->next;
        }
        cleanMemory(node);
        return 0;
}
