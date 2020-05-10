#pragma once

#include <network/ServerNetwork.h>
#include <game/GameState.h>
#include <unordered_map>
#include <objects/GameObject.h>
#include <game/GameProcessor.h>
#include <util/PrintUtil.h>
#include <util/MessageBuilder.h>
#include <thread>
#include <chrono>

class ServerGame
{
public:

    ServerGame(int port);

    ~ServerGame();

private:
    ServerNetwork server;

    GameState gameState;

    GameProcessor processor;

    void run();

    void process();
    
    void acceptCallback(int);
};