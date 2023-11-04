
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct List_s {
    struct List_s*next;
    char* value;
} List;

List* initElem(char* s){
    List *elem = malloc(sizeof(List));
    int n = strlen(s) + 1;
    elem->value = malloc(n * sizeof(char));
    strcpy(elem->value,s);
    elem->next = NULL;
    return elem;
}

List* addElem(List* arr, char *s){
    if (arr != NULL){
        List *tmp = arr;
        while (tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = initElem(s);
        return arr;
    }
    else {
        return initElem(s);
    }
}
void listPrint(List* arr){
    while(arr != NULL){
        printf("%s", arr->value);
        arr = arr->next;
    }
}

void freeList(List* arr){
    while(arr != NULL){
        List* tmp = arr->next;
        free(arr);
        arr = tmp;
    }
}

int main(){
    char buffer[BUFSIZ];
    List *arr = NULL;
    while (fgets(buffer,BUFSIZ,stdin)!= NULL &&
           buffer[0] != '.' && buffer[strlen(buffer)-1] == '\n'){
        arr = addElem(arr,buffer);
    }
    if (buffer[strlen(buffer)-1] != '\n' && buffer[0] != '.'){
        perror("string size exceeds BUFSIZ");
    }
    listPrint(arr);
    freeList(arr);
    return 0;
}
