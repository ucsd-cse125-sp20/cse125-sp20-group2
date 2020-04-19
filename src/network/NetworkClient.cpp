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

void NetworkClient::send(Game::ClientMessage clientMessage)
{
    // Send size and data
    size_t msgSize = clientMessage.ByteSizeLong();
    size_t bufSize = msgSize + sizeof(size_t);
    char buf[bufSize];
    memcpy(buf, &msgSize, sizeof(size_t));
    clientMessage.SerializeToArray(buf + sizeof(size_t), msgSize);

    NetworkService::sendMessage(this->ConnectSocket, buf, bufSize);
}

std::string NetworkClient::read() 
{
// int receivedBytes = NetworkService::receiveMessage(this->ConnectSocket, network_data, DEFAULT_BUFLEN);
// if (receivedBytes > 0) {
//     return std::string (network_data);
// } else {
//     return 0;
// }
}