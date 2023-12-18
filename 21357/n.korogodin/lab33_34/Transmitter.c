#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void frame_data(int connection_id, char* data, char* frame) {
    sprintf(frame, "\x7E%c%s\x7E", connection_id, data);
}

void encode_data(const char* input, char* output) {
    size_t input_len = strlen(input);
    size_t output_index = 0;

    for (size_t i = 0; i < input_len; ++i) {
        if (input[i] == 0x7E) {
            // Replace 01111110b with 01111101b 01111110b
            output[output_index++] = 0x7D;
            output[output_index++] = 0x7E;
        }
        else if (input[i] == 0x7D) {
            // Replace 01111101b with 01111101b 01111101b
            output[output_index++] = 0x7D;
            output[output_index++] = 0x7D;
        }
        else {
            output[output_index++] = input[i];
        }
    }

    output[output_index] = '\0';
}


void transmit_data(int transmitter_socket, int connection_id, char* data) {
    char frame[BUFFER_SIZE];
    frame_data(connection_id, data, frame);
    write(transmitter_socket, frame, strlen(frame));
}

int main() {
    int transmitter_socket, receiver_socket;
    struct sockaddr_in transmitter_addr, receiver_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    char encoded_data[BUFFER_SIZE];

    transmitter_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (transmitter_socket == -1) {
        perror("Error creating transmitter socket");
        exit(EXIT_FAILURE);
    }

    transmitter_addr.sin_family = AF_INET;
    transmitter_addr.sin_addr.s_addr = INADDR_ANY;
    transmitter_addr.sin_port = htons(12345);

    bind(transmitter_socket, (struct sockaddr*)&transmitter_addr, sizeof(transmitter_addr));
    listen(transmitter_socket, 1);

    printf("Transmitter listening on 127.0.0.1:12345\n");

    receiver_socket = accept(transmitter_socket, (struct sockaddr*)&receiver_addr, &addr_size);
    printf("Connection established with receiver\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        encode_data(buffer, encoded_data);
        transmit_data(receiver_socket, 1, encoded_data);
    }

    close(transmitter_socket);
    close(receiver_socket);

    return 0;
}
