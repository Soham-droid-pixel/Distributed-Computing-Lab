#include <iostream>
#include <thread>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool running = true;

// Thread function to receive messages
void receive_messages(SOCKET socket) {
    char buffer[1024];
    while (running) {
        memset(buffer, 0, 1024);
        int bytes = recv(socket, buffer, 1024, 0);
        if (bytes > 0) {
            cout << "\n[Group]: " << buffer << endl;
            cout << "You: " << flush; 
        } else {
            running = false;
        }
    }
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET client_socket;
    struct sockaddr_in server_addr;
    string name;

    cout << "Enter Name (Master/Slave): ";
    cin >> name;
    cin.ignore(); // Clear buffer

    // 2. Create Socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Connect
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cout << "Connection failed. Is the server running?" << endl;
        return 1;
    }

    cout << "Connected to Group! Start typing..." << endl;

    // 4. Start Receiver Thread
    thread t(receive_messages, client_socket);
    t.detach();

    // 5. Send Loop
    string msg;
    while (running) {
        cout << "You: ";
        getline(cin, msg);
        
        if (msg == "exit") break;
        
        string full_msg = name + ": " + msg;
        send(client_socket, full_msg.c_str(), full_msg.length(), 0);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}