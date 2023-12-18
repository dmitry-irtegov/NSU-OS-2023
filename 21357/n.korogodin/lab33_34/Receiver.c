#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void unframe_data(char* data, char* decoded_data) {
    strncpy(decoded_data, data + 2, strlen(data) - 3);
}
void decode_data(const char* input, char* output) {
    size_t input_len = strlen(input);
    size_t output_index = 0;

    for (size_t i = 1; i < input_len-1; ++i) {
        if (input[i] == 0x7D && input[i + 1] == 0x7E) {
            output[output_index++] = 0x7E;
            ++i;
        }
        else if (input[i] == 0x7D && input[i + 1] == 0x7D) {
            output[output_index++] = 0x7D;
            ++i;
        }
        else if (input[i] == '\n') {
            output[output_index++] = '\n';
            break;
        }
        else {
            output[output_index++] = input[i];
        }
    }

    //output[output_index-1] = '\0';
}
int main() {
    int transmitter_socket, receiver_socket;
    struct sockaddr_in transmitter_addr;
    char buffer[BUFFER_SIZE];
    char encoded_data[BUFFER_SIZE];
    int len;

    receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_socket == -1) {
        perror("Error creating receiver socket");
        exit(EXIT_FAILURE);
    }

    transmitter_addr.sin_family = AF_INET;
    transmitter_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    transmitter_addr.sin_port = htons(12345);

    if (connect(receiver_socket, (struct sockaddr*)&transmitter_addr, sizeof(transmitter_addr))) {
        printf("Connection established with transmitter\n");
    }

    while (1) {
        if((len = read(receiver_socket, encoded_data, BUFFER_SIZE - 1)) > 0) {
            memset(buffer, 0, sizeof(buffer));
            decode_data(encoded_data, buffer);
            printf("Received data: %s", buffer);
        }
    }

    close(receiver_socket);

    return 0;
}
