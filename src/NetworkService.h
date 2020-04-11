#pragma once 

#include <WinSock2.h>
#include <Windows.h>

#define MAX_PACKET_SIZE 1000000
#define DEFAULT_BUFLEN 512

class NetworkService
{
public:
    static int sendMessage(SOCKET socket, char* message, int messageSize);
    static int receiveMessage(SOCKET socket, char* message, int messageSize);
};