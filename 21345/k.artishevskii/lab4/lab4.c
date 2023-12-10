#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
		perror("Malloc error");
		exit(1);
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
	if(node == NULL){
		perror("Malloc error");
		exit(1);
	}
	node->next = NULL;
	return node;
}

void insert(List* list, char* str){
	if(list == NULL || str == NULL){
		return;
	}
	Node* node = createNode();
	if(list->first){
		node->next = list->first;
	}
	node->str = (char*)malloc(sizeof(char) * strlen(str) + 1);
	if(node->str == NULL){
		free(node);
		perror("Malloc error");
		exit(1);
	}
	memcpy(node->str, str, strlen(str) + 1);
	list->first = node;
}

char* getString(char* buf, size_t buf_size){
	if(buf == NULL){
		return NULL;
	}
	char* string = fgets(buf, buf_size, stdin);
	return string;
}

void reversePrint(Node* node){
	if(node->next == NULL){
		printf("%s", node->str);
	}else{
		reversePrint(node->next);
		printf("%s", node->str);
	}
}

int main(int argc, char* argv[]){
	List* list = createList();
	if(list == NULL){
		perror("Couldn't allocate memory");
		return -1;
	}
	char buf[MAX_SIZE];
	bool create_new_string = true;
	while(1){
		char* string = getString((char*) &buf, MAX_SIZE);
		if(create_new_string && string[0] == '.'){
			break;
		}
		create_new_string = string[strlen(string) - 1] == '\n';
		insert(list, string);
	}
	reversePrint(list->first);
	clearList(list);
}
