#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include "NetworkClient.h"

NetworkClient::NetworkClient(char* host, int port)
{
    this->host = host;
    this->port = port;
    setup();
}

NetworkClient::~NetworkClient()
{
    closesocket(ConnectSocket);
    WSACleanup();
}

void NetworkClient::setup()
{
    WSADATA wsaData;
    int res = 0;
    
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    // Initialize Winsock
    res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (res != 0) {
        printf("WSAStartup failed xd with error: %d\n", res);
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    std::string port_string = std::to_string(this->port);
    res = getaddrinfo(host, port_string.c_str(), &hints, &result);
    if (res != 0) {
        printf("getaddrinfo failed with error: %d\n", res);
        WSACleanup();
        return;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return;
        }

        // Connect to server.
        res = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (res == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }
    
}