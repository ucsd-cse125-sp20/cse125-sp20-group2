#pragma once 

#include <schema/Game.pb.h>
#include <game/GameState.h>
#include <deque>

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
    void process(unsigned int, Game::ClientMessage);
    
    std::deque<Game::ServerMessage*> messages;
};