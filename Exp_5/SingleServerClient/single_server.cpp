#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind and Listen
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Single Server Listening on Port 8080..." << endl;

    // Accept one connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    // Read message
    recv(new_socket, buffer, 1024, 0);
    cout << "Client says: " << buffer << endl;

    // Send reply
    const char *hello = "Hello from Server";
    send(new_socket, hello, strlen(hello), 0);
    cout << "Reply sent to client." << endl;

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}