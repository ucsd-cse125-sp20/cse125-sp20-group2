#include "NetworkService.h"

int NetworkService::sendMessage(SOCKET socket, char* message, int messageSize)
{
    return send(socket, message, messageSize, 0);
}

int NetworkService::receiveMessage(SOCKET socket, char* message, int messageSize)
{
    return recv(socket, message, messageSize, 0);
}