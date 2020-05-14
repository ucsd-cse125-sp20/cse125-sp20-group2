#pragma once

#include <network/ServerNetwork.h>
#include <game/GameState.h>
#include <unordered_map>
#include <objects/GameObject.h>
#include <game/GameProcessor.h>

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
    
    void onClientConnect(int);
};