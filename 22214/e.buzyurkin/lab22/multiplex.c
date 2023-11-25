#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#define TIMEOUT 1000

typedef struct Node_s {
    struct Node_s* next;
    struct Node_s* prev;
    int fd;
    char* filename;
} Node;


Node* pushList(Node* last, int fd, char* filename){
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL){
        perror("An error has occured");
        exit(1);
    }

    newNode->next = last->next;
    newNode->prev = last;

    last->next->prev = newNode;
    last->next = newNode;

    newNode->fd = fd;
    newNode->filename = filename;

    return newNode;
}


Node* removeNode(Node* node) {
    if (node->next == node) {
        return NULL;
    }

    Node* nextNode = node->next;
    Node* prevNode = node->prev;

    nextNode->prev = prevNode;
    prevNode->next = nextNode;

    return nextNode;
}


int readline(char* buf, int len, int fd) {
    char c;
    int i = 0;

    while(read(fd, &c, 1) > 0 && i < len) {
        if (c == '\n') {
            buf[i++] = c;
            break;
        }

        buf[i++] = c;
    }

    return i;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No files provided\n");
        exit(1);
    }

    Node* head = malloc(sizeof(Node));
    if (head == NULL) {
        perror("An error has occured");
        exit(1);
    }

    head->fd = open(argv[1], O_RDONLY);
    if (head->fd == -1) {
        perror("Couldnt open the file");
        exit(1);
    }

    head->filename = argv[1];
    head->next = head;
    head->prev = NULL;

    Node* last = head;

    for (int i = 2; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("Couldnt open the file");
            exit(1);
        }

        pushList(last, fd, argv[i]);
    }

    char buf[BUFSIZ];
    int timeouts_in_a_row = 0;

    while (1) {
        struct pollfd fds;

        fds.fd = head->fd;
        fds.events = POLLIN;

        printf("reading from file \'%s\' : ", head->filename);
        
        int poll_return = poll(&fds, 1, TIMEOUT);
        switch (poll_return) {
            case -1:
                perror("An error has occured");
                exit(1);

            case 0:
                timeouts_in_a_row++;
                printf("\n");
                break;
            case 1:
                timeouts_in_a_row = 0;

                if (fds.revents & POLLIN) {
                    int bytes_read = readline(buf, BUFSIZ, fds.fd);
                    if (bytes_read > 0) {
                        buf[bytes_read] = 0;
                        printf("%s\n", buf);
                    }
                    else {
                        close(head->fd);
                        printf("file ended: closed file \n");
                        head = removeNode(head);
                    }
                }
                else {
                    close(head->fd);
                    printf("error while polling : closed file \n");
                    head = removeNode(head);
                }
                break;
        }

        if (head == NULL) {
            break;
        }

        if (timeouts_in_a_row == 3) {
            close(head->fd);
            printf("closed file \'%s\' due to 3 timeouts\n", head->filename);
            head = removeNode(head);
            break;
        }

        head = head->next;
    }


    return 0;
}