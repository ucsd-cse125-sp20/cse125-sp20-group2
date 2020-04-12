#include <stdio.h>
#include <string>
#include "NetworkClient.h"

NetworkClient::NetworkClient(char* host, int port)
{
    this->ConnectSocket = NetworkService::createClientSocket(host, port);
    if (this->ConnectSocket == INVALID_SOCKET) {
        exit(ERROR);
    }
}

NetworkClient::~NetworkClient()
{
    // shutdown(ConnectSocket, SD_SEND);
    closesocket(ConnectSocket);
    WSACleanup();
}

void NetworkClient::send(std::string msg)
{
    int sentBytes = NetworkService::sendMessage(this->ConnectSocket, msg.c_str(), msg.size());
}

std::string NetworkClient::read() 
{
    int receivedBytes = NetworkService::receiveMessage(this->ConnectSocket, network_data, DEFAULT_BUFLEN);
    if (receivedBytes > 0) {
        return std::string (network_data);
    } else {
        return 0;
    }
}