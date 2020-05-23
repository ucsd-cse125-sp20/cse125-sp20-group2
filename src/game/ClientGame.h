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

/// TODO: remove this
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
        unsigned int round = 0;

    private:
        NetworkClient client;
        Window window;
        void sendMsgs();
        void receiveUpdates();
        void updateGameState();
        void processInput();
};