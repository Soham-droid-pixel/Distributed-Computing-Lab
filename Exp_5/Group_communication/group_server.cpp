#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link Windows Socket Library
#pragma comment(lib, "ws2_32.lib")

using namespace std;

vector<SOCKET> clients; // Use SOCKET type for windows

// Broadcast message to all clients except sender
void broadcast_message(string message, SOCKET sender_socket) {
    for (SOCKET client_socket : clients) {
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.length(), 0);
        }
    }
}

// Handle individual client communication
void handle_client(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        
        if (bytes_received <= 0) {
            cout << "Client disconnected." << endl;
            closesocket(client_socket);
            
            // Remove from list safely
            // (In a real app, use mutex/locks here for thread safety)
            for (auto it = clients.begin(); it != clients.end(); ++it) {
                if (*it == client_socket) {
                    clients.erase(it);
                    break;
                }
            }
            break;
        }

        string msg(buffer);
        cout << "Broadcasting: " << msg << endl;
        broadcast_message(msg, client_socket);
    }
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_size = sizeof(client_addr);

    // 2. Create Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        return 1;
    }

    // 3. Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081); // Port 8081
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed." << endl;
        return 1;
    }

    // 4. Listen
    listen(server_socket, 10);
    cout << "Multi-Client Server started on Port 8081..." << endl;

    // 5. Accept Loop
    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        
        if (client_socket != INVALID_SOCKET) {
            clients.push_back(client_socket);
            cout << "New Group Member joined!" << endl;
            
            // Create a thread for the new client
            thread t(handle_client, client_socket);
            t.detach(); 
        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}