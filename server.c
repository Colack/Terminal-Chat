#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_MESSAGE_LEN 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_MESSAGE_LEN] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Create socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Bind socket to a specific port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Listen failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection and start handling messages
    while (1) {
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (int*)&addrlen)) == INVALID_SOCKET) {
            printf("Accept failed. Error Code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        // Read message from client
        recv(new_socket, buffer, MAX_MESSAGE_LEN, 0);
        printf("Received message from client: %s\n", buffer);

        // Clear message buffer
        memset(buffer, 0, MAX_MESSAGE_LEN);

        // Close connection to client
        closesocket(new_socket);
    }

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
