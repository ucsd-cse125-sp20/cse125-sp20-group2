#include <iostream>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include "NetworkServer.h"

// TODO:
//  Handle clientSocket disconnection

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

unsigned int ServerNetwork::getNextId()
{
    return nextId++;
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

        int id = getNextId();
        std::cout << "Accepted Client #" << id << std::endl;
        sessions.insert(std::pair<unsigned int, SOCKET>(id, ClientSocket));
    }
}

std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> ServerNetwork::readAllMessages()
{
    std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> map;
    std::map<unsigned int, SOCKET>::iterator iter;

    if (sessions.empty()) {
        return map;
    }
    
    for (iter = sessions.begin(); iter != sessions.end(); iter++) {
        unsigned int clientId = iter->first;
        SOCKET socket = iter->second;
        int bytesReceived = 1;

        // Read message into a queue to parse
        while (bytesReceived > 0) {
            bytesReceived = NetworkService::receiveMessage(socket, network_data, DEFAULT_BUFLEN);
            if (bytesReceived > 0) {
                for (int i = 0; i < bytesReceived; i++) buffers[clientId].push_back(network_data[i]);
            }
        }

        // First, need to figure out the vector<char> size
        std::vector<char> & buffer = buffers[clientId];
        bool success = true;
        while (success) {
            if (buffer.size() <= sizeof(size_t)) {
                success = false;
                continue;
            }

            // This is the expected size of the ClientMessage
            size_t expectedSize;
            memcpy(&expectedSize, &buffer[0], sizeof(size_t));

            // This is the actual size we have from the buffer
            size_t actualSize = buffer.size() - sizeof(size_t);

            // Case 1: If it hasn't transferred enough, skip
            if (actualSize < expectedSize) {
                success = false;
                continue;
            }
            
            // Case 2: Read the msg, parse into object
            buffer.erase(buffer.begin(), buffer.begin() + sizeof(size_t));            
            Game::ClientMessage clientMessage;

            // Copy contents into array
            char objBuf[expectedSize];
            std::copy(buffer.begin(), buffer.begin() + expectedSize, objBuf);

            // Parse into client message
            bool result = clientMessage.ParseFromArray(objBuf, expectedSize);
            
            // Erase the message from buffer
            buffer.erase(buffer.begin(), buffer.begin() + expectedSize);

            // We received corrupt data that should not have happened
            // Assume client is friendly and data is correctly formatted
            if (!result) {
                std::cerr << "Something bad happened!" << std::endl;
                exit(1);
            }

            map[clientId].push_back(clientMessage);
        }
    }
    return map;
}

void ServerNetwork::send(unsigned int clientID, Game::ServerMessage message)
{
    // TODO: Fix
    // SOCKET clientSocket = this->sessions[clientID];
    // NetworkService::sendMessage(clientSocket, msg.c_str(), msg.size()); 
}

void ServerNetwork::sendToAll(Game::ServerMessage message) 
{
    for (auto pair: this->sessions) {
        unsigned int clientId = pair.first;
        this->send(clientId, message);
    }
}