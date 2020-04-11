#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include "NetworkServer.h"

// TODO:
// Accept is blocking
// Recv is also blocking
// one client one server
// one thread for accept/recv, game logic is on main thread
// 


ServerNetwork::ServerNetwork(int port)
{
    this->port = port;
    setup();
    // I think this works (pray)
    std::thread acceptThread (&ServerNetwork::startAccepting, this);
    std::thread recvThread (&ServerNetwork::readAllMessages, this);
    acceptThread.join(); // TODO: Join somewhere else
}

ServerNetwork::~ServerNetwork()
{
    // TODO: Go through map of client sockets and call shutdown and closesocket
    closesocket(ListenSocket);
    WSACleanup();
}

void ServerNetwork::setup()
{
    WSADATA wsaData;
    int result;

    struct addrinfo *res = NULL;
    struct addrinfo hints;

    // Initialize WinSock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    // Convert port to string
    std::string port_string = std::to_string(port);
    result = getaddrinfo(NULL, port_string.c_str(), &hints, &res);
    if (result != 0) {
        printf("getaddrinfo failed with error: %d\n", result);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to the server
    ListenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
        return;
    }

    // Set the socket I/O mode: In this case FIONBIO
    // enables or disables the blocking mode for the 
    // socket based on the numerical value of iMode.
    // If iMode = 0, blocking is enabled; 
    // If iMode != 0, non-blocking mode is enabled.
    u_long iMode = 1;
    result = ioctlsocket(ListenSocket, FIONBIO, &iMode);
    if (result != NO_ERROR) {
        printf("failed to set nonblocking\n");
        freeaddrinfo(res);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    result = bind(ListenSocket, res->ai_addr, (int) res->ai_addrlen);
    if (result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(res);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(res);

    // Places socket into listening state
    result = listen(ListenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
}

int ServerNetwork::getNextID()
{
    return next_id++;
}

void ServerNetwork::startAccepting()
{
    std::cout << "Accepting Clients" << std::endl;
    while (1) {
        SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                return;
            }
            continue;
        }

        int id = getNextID();
        std::cout << "Accepted Client #" << id << std::endl;
        sessions.insert(std::pair<unsigned int, SOCKET>(id, ClientSocket));
    }
}


void ServerNetwork::readAllMessages()
{
    int result;
    while (1) {
        std::map<unsigned int, SOCKET>::iterator iter;

        if (sessions.empty()) {
            continue;
        }
        
        for (iter = sessions.begin(); iter != sessions.end(); iter++) {
            int client_id = iter->first;
            SOCKET socket = iter->second;
            int byte_received = NetworkService::receiveMessage(socket, network_data, DEFAULT_BUFLEN);
            if (byte_received > 0) {
                std::cout << "Client #" << client_id << network_data << std::endl;
            }
        }
    }
}