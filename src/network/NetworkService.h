#pragma once 

#include <WinSock2.h>
#include <Windows.h>

#define DEFAULT_BUFLEN 512

/**
 * C++ wrapper for C socket
 * */
class NetworkService
{
public:
    static int sendMessage(SOCKET socket, const char* message, int messageSize);
    static int receiveMessage(SOCKET socket, char* message, int messageSize);

    static SOCKET createServerSocket(int port);
    static SOCKET createClientSocket(char* host, int port);
};