#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER_SIZE 1048320
typedef struct Node{
        char * string;
        struct Node * next;
} Node;

void cleanMemory(Node * startNode);

Node *initNode(char *str){
        Node * startNode = (Node *)malloc(sizeof(Node));
        if (startNode == NULL){
                perror("Memory isn't allocated");
                return NULL;
        }
        startNode->string = (char *)malloc((strlen(str) + 1) * sizeof(char));
        if (startNode->string == NULL){
                perror("Memory for string isn't allocated");
                free(startNode);
                return NULL;
        }
        for (int i = 0; i < strlen(str) + 1; i++){
                startNode->string[i] = str[i];
        }
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

void cleanMemory(Node * startNode){
        Node * tmp;
        while(startNode != NULL){
                tmp = startNode;
                startNode = startNode->next;
                free(tmp->string);
                free(tmp);
        }
}



int main() {
        char buffer[MAX_BUFFER_SIZE + 1];
        Node * node = NULL;
        while(1){
                if (fgets(buffer, MAX_BUFFER_SIZE, stdin) == NULL){
                        break;
                }
                if (buffer[0] == '.'){
                        break;
                }
                buffer[strlen(buffer) - 1] = '\0';
                node = addToEnd(buffer, node);
        }

        int stringNumber = 1;
        Node * tmp;
        tmp = node;
        while(tmp != NULL){
                printf("string number %d: \"%s\"\n",stringNumber, tmp->string);
                tmp = tmp->next;
                stringNumber++;
        }
        cleanMemory(node);
        return 0;
}
