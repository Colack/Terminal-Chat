#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_MESSAGE_LEN 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_address;
    char message[MAX_MESSAGE_LEN] = {0};
    char buffer[MAX_MESSAGE_LEN] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Connection failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Prompt user to enter message
        printf("Enter message to send to server: ");
        fgets(message, MAX_MESSAGE_LEN, stdin);

        // Send message to server
        send(sock, message, strlen(message), 0);
        printf("Message sent to server\n");
    }

    // Close socket
    closesocket(sock);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
