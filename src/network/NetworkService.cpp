#include "NetworkService.h"
#include <ws2tcpip.h>
#include <string>
#include <iostream>

int NetworkService::sendMessage(SOCKET socket, const char* message, int messageSize)
{
    return send(socket, message, messageSize, 0);
}

int NetworkService::receiveMessage(SOCKET socket, char* message, int messageSize)
{
    return recv(socket, message, messageSize, 0);
}

SOCKET NetworkService::createServerSocket(int port)
{
    WSADATA wsaData;
    int result;

    struct addrinfo *res = NULL;
    struct addrinfo hints;

    // Initialize WinSock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed with error:" << result << std::endl;
        return INVALID_SOCKET;
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
        std::cout << "getaddrinfo failed with error:" << result << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Create a SOCKET for connecting to the server
    SOCKET ListenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error:" << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Set the socket I/O mode: In this case FIONBIO
    // enables or disables the blocking mode for the 
    // socket based on the numerical value of iMode.
    // If iMode = 0, blocking is enabled; 
    // If iMode != 0, non-blocking mode is enabled.
    u_long iMode = 1;
    result = ioctlsocket(ListenSocket, FIONBIO, &iMode);
    if (result != NO_ERROR) {
        std::cout << "failed to set nonblocking" << std::endl;
        freeaddrinfo(res);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Setup the TCP listening socket
    result = bind(ListenSocket, res->ai_addr, (int) res->ai_addrlen);
    if (result == SOCKET_ERROR) {
        std::cout << "bind failed with error:" << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    freeaddrinfo(res);

    // Places socket into listening state
    result = listen(ListenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cout << "listen failed with error:" << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return ListenSocket;
}

SOCKET NetworkService::createClientSocket(char* host, int port)
{
    WSADATA wsaData;
    int res = 0;
    
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    // Initialize Winsock
    res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (res != 0) {
        std::cout << "WSAStartup failed xd with error:" << res << std::endl;
        return INVALID_SOCKET;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    std::string port_string = std::to_string(port);
    res = getaddrinfo(host, port_string.c_str(), &hints, &result);
    if (res != 0) {
        std::cout << "getaddrinfo failed with error:" << res << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    SOCKET ConnectSocket;

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            std::cout << "socket failed with error:" << WSAGetLastError() << std::endl;
            WSACleanup();
            return INVALID_SOCKET;
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
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }
    
    return ConnectSocket;
}