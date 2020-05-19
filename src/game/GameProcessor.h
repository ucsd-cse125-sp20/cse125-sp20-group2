#pragma once 

#include <schema/Game.pb.h>
#include <game/GameState.h>
#include <game/MovementProcessor.h>
#include <deque>
#include <objects/Map.h>
#include <util/MapBuilder.h>
#include <util/RecipeBuilder.h>
#include <util/MessageBuilder.h>

/**
 * This processes information from the server and modifies game state.
 * */
class GameProcessor
{
private:
    GameState* state;

public:
    GameProcessor(GameState*);
    
    ~GameProcessor();
    
    /**
     * This should be called to remove unnecessary duplicate messages
     * */
    // void preprocess();

    /**
     * This processes a client message for a client id.
     * This will modify GameState.
     * */
    void process(unsigned int, Game::ClientMessage, int);
    
    // These are messages send to everyone
    std::deque<Game::ServerMessage*> messages;

    // These are messages sent to specific clients
    std::unordered_map<unsigned int, std::deque<Game::ServerMessage*>> specificMessages;
    
};