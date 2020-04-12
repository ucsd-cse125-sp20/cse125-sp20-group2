#include <iostream>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include "NetworkServer.h"

ServerNetwork::ServerNetwork(int port)
{
    this->listenSocket = NetworkService::createServerSocket(port);
    this->threadAccept = std::thread (&ServerNetwork::startAccepting, this);
}

ServerNetwork::~ServerNetwork()
{
    std::map<unsigned int, SOCKET>::iterator mapIter;
    for (mapIter = sessions.begin(); mapIter != sessions.end(); mapIter++)
    {
        SOCKET currSocket = mapIter->second;
        // let peer know that server is shutting down
        shutdown(currSocket, SD_SEND);
        closesocket(currSocket);
    }
    closesocket(listenSocket);
    WSACleanup();
}

unsigned int ServerNetwork::getNextID()
{
    return nextID++;
}

void ServerNetwork::startAccepting()
{
    std::cout << "Accepting Clients" << std::endl;
    while (1) {
        SOCKET ClientSocket = accept(listenSocket, NULL, NULL);
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            continue;
        }

        if (ClientSocket == INVALID_SOCKET) {
            std::cout << "Accept failed with error:" << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }

        int id = getNextID();
        std::cout << "Accepted Client #" << id << std::endl;
        sessions.insert(std::pair<unsigned int, SOCKET>(id, ClientSocket));
    }
}

std::unordered_map<unsigned int, std::vector<std::string>> ServerNetwork::readAllMessages()
{
    std::unordered_map<unsigned int, std::vector<std::string>> map;
    std::map<unsigned int, SOCKET>::iterator iter;

    if (sessions.empty()) {
        return map;
    }
    
    for (iter = sessions.begin(); iter != sessions.end(); iter++) {
        unsigned int client_id = iter->first;
        SOCKET socket = iter->second;
        int byteReceived = 1;
        std::vector<std::string> msgs;
        while (byteReceived > 0) {
            byteReceived = NetworkService::receiveMessage(socket, network_data, DEFAULT_BUFLEN);
            if (byteReceived > 0) {
                std::string data (network_data);
                msgs.insert(msgs.end(), data);
            }
        }
        map.insert(std::pair<unsigned int, std::vector<std::string>>(client_id, msgs));
    }
    return map;
}

// TODO: What if clientSocket is disconnected?????
void ServerNetwork::send(unsigned int clientID, std::string msg)
{
    SOCKET clientSocket = this->sessions[clientID];
    NetworkService::sendMessage(clientSocket, msg.c_str(), msg.size()); 
}

void ServerNetwork::sendToAll(std::string msg) 
{
    for (std::pair<unsigned int, SOCKET> currPair : this->sessions)
    {
        this->send(currPair.first, msg);
        // SOCKET clientSocket = currPair.second;
        // NetworkService::sendMessage(clientSocket, msg.c_str(), msg.size());
    }
}