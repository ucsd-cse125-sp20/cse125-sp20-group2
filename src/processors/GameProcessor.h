#pragma once 

#include <schema/Game.pb.h>
#include <game/GameState.h>
#include <processors/MovementProcessor.h>
#include <deque>
#include <objects/Map.h>
#include <util/MapBuilder.h>
#include <util/RecipeBuilder.h>
#include <util/MessageBuilder.h>
#include <google/protobuf/util/message_differencer.h>
#include <game/ServerGame.h>

class ServerGame;

/**
 * This processes information from the server and modifies game state.
 * */
class GameProcessor
{
    public:
        /**
         * Initializes the game state
         * */
        static void initGameState(GameState* gameState);

        /**
         * This processes a client message for a client id.
         * This will modify GameState.
         * */
        static void Process(unsigned int, Game::ClientMessage, ServerGame*);

        /**
         * Used to spawn another ingredient on the map randomly
         * */
        static IngredientObject* spawnIngredient(GameState* gameState, Recipe* recipe);
};