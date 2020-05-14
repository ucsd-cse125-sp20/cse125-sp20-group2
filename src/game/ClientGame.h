#pragma once

#include <network/NetworkClient.h>
#include <iostream>
#include <util/PrintUtil.h>
#include <graphics/window.h>
#include <objects/Player.h>

class ClientGame {
    public:
        ClientGame(std::string IP, int port);
        ~ClientGame();
        void runGame();

        // This is the client id.
        // NOTE: This does NOT correlate directly with any objects!!!
        unsigned int clientId;

        // This is the object id.
        // NOTE: This is the id that correlates to this client's player object. 
        unsigned int objectId;
    private:
        Window window;
        NetworkClient client;
        void sendMsgs();
        void receiveUpdates();
        void updateGameState();
        void processInput();
};