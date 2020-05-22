#pragma once 

#include <schema/Game.pb.h>
#include <game/GameState.h>
#include <game/MovementProcessor.h>
#include <deque>
#include <objects/Map.h>
#include <util/MapBuilder.h>
#include <util/RecipeBuilder.h>
#include <util/MessageBuilder.h>
#include <google/protobuf/util/message_differencer.h>

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
     * Removes unecessary movement duplicates
     * */
    void Preprocess(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> &);

    /**
     * This processes a client message for a client id.
     * This will modify GameState.
     * */
    void Process(unsigned int, Game::ClientMessage, int);
    
    // These are messages send to everyone
    std::deque<Game::ServerMessage*> messages;

    // These are messages sent to specific clients
    std::unordered_map<unsigned int, std::deque<Game::ServerMessage*>> specificMessages;
    
};