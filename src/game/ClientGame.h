#pragma once

#include <network/NetworkClient.h>
#include <iostream>
#include <util/PrintUtil.h>
#include <graphics/window.h>
#include <objects/Player.h>
#include <glm/gtx/string_cast.hpp>
#include <chrono>
#include <thread>
#include <util/MessageBuilder.h>
#include <util/Config.h>
#include <util/MapBuilder.h>

class ClientGame {
    public:
        ClientGame(std::string IP, int port);
        ~ClientGame();
        void runGame();
        int clientID;
    private:
        NetworkClient client;
        Window window;
        void sendMsgs();
        void receiveUpdates();
        void updateGameState();
        void processInput();
};