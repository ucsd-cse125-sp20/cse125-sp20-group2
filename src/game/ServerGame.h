#pragma once

#include <network/ServerNetwork.h>
#include <game/GameState.h>
#include <unordered_map>
#include <objects/GameObject.h>
#include <util/PrintUtil.h>
#include <util/MessageBuilder.h>
#include <thread>
#include <chrono>

class ServerGame
{
    public:

        ServerGame(int port);

        ~ServerGame();

        // The world
        GameState gameState;

        int getTick();

        // These are messages send to everyone
        std::deque<Game::ServerMessage*> messages;

        // These are messages sent to specific clients
        std::unordered_map<unsigned int, std::deque<Game::ServerMessage*>> specificMessages;

        /**
         * Runs the server.
         */
        void run();

    private:

        const int TICK = 30;

        ServerNetwork server;

        /**
         * Cleans the messages before using them
         * */
        void preprocess(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> & clientMsgs);

        /*
        * Uses the messages to update the game state,
        * using the appropriate processor
        * */
        void process(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> & msgMap);

        /*
        * Use to process time sensitive events
        * */
        void update();

        /**
         * Sends messages for specific players and
         * messages for all players
         * */
        void sendPendingMessages();
        
        /**
        * Called from server network when it accepts a new client
        * */
        void onClientConnect(int);

        /**
        * Called when there is a change of round.
        * This should send out the message that there is a round change to the client.
        * This should also initialize the respective map and objects for the round
        * and send out the map and object messages.
        * */
        void onRoundChange();
};