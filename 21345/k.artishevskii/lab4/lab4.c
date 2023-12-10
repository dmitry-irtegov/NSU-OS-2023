#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 255

typedef struct Node {
	char* str;
	struct Node *next;
} Node;

typedef struct List{
	Node* first;
} List;

List* createList(){
	List* list = (List*)malloc(sizeof(List));
	if(list == NULL){
		return NULL;
	}
	list->first = NULL;
	return list;
}

void clearList(List* list){
	if(list){
		Node* cur = list->first;
		while(cur){
			if(cur->str){
				free(cur->str);
			}
			Node* tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(list);
}

Node* createNode(){
	Node* node = (Node*)malloc(sizeof(Node));
	if(node){
		node->next = NULL;
	}
	return node;
}

void insert(List* list, char* str){
	if(list == NULL || str == NULL){
		return;
	}
	Node* node = createNode();
	if(node == NULL){
		return;
	}
	if(list->first){
		node->next = list->first;
	}
	node->str = (char*)malloc(sizeof(char) * strlen(str) + 1);
	if(node->str == NULL){
		return;
	}
	memcpy(node->str, str, strlen(str) + 1);
	list->first = node;
}

char* getString(char* buf){
	if(buf == NULL){
		return NULL;
	}
	char* string = fgets(buf, MAX_SIZE, stdin);
	return string;
}

void printList(List* list){
	if(list){
		Node* cur = list->first;
		while(cur){
			printf("%s", cur->str);
			cur = cur->next;
		}
	}
}

int main(int argc, char* argv[]){
	List* list = createList();
	if(list == NULL){
		perror("Couldn't allocate memory");
		return -1;
	}
	char buf[MAX_SIZE];
	while(1){
		char* string = getString((char*) &buf);
		if(string[0] != '.'){
			insert(list, string);
		}
		else {
			break;
		}
	}
	printList(list);
	clearList(list);
}
