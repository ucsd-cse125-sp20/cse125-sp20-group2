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

    GameProcessor gameProcessor;

    void run();

    void process();

    /**
     * Sends messages for specific players and
     * messages for all players
     * */
    void sendPendingMessages();

    /**
     * Updates world as if game was in lobby state
     * */
    void updateLobbyState(unsigned int, std::vector<Game::ClientMessage>);

    /**
     * Updates world as if game was in waiting state
     * */
    void updateWaitingState(unsigned int, std::vector<Game::ClientMessage>);
    
    /**
     * Updates world as if game was in playing state
     * */
    void updatePlayingState(unsigned int, std::vector<Game::ClientMessage>);
    
    /**
    * Called from server network when it accepts a new client
    * */
    void onClientConnect(int);
};