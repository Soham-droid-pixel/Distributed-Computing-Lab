#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link the Winsock library
#pragma comment(lib, "ws2_32.lib")

// Suppress warnings for using inet_addr (older simple function)
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

using namespace std;

int main() {
    // 1. Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cout << "WSAStartup failed" << endl;
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *hello = "Hello from Single Client";

    // 2. Create Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        WSACleanup();
        return -1;
    }

    // 3. Configure Server Address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to Localhost

    // 4. Connect to Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed. Is the server running?" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // 5. Send Message
    send(sock, hello, strlen(hello), 0);
    cout << "Message sent to server." << endl;

    // 6. Receive Reply
    int valread = recv(sock, buffer, 1024, 0);
    if (valread > 0) {
        cout << "Server replies: " << buffer << endl;
    }

    // 7. Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}