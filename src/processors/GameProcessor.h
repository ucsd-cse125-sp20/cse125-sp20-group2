#pragma once 

#include <schema/Game.pb.h>
#include <game/GameState.h>
#include <processors/MovementProcessor.h>
#include <deque>
#include <util/MapBuilder.h>
#include <util/RecipeBuilder.h>
#include <util/MessageBuilder.h>
#include <util/Config.h>
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
         * Initializes the dungeon phase
         * */
        static void initDungeonPhase(GameState* gameState);

        /*
        * Initialize the kitchen phase
        * */
        static void initKitchenPhase(GameState* gameState);

        /**
         * This processes a client message for a client id.
         * This will modify GameState.
         * */
        static void Process(unsigned int, Game::ClientMessage, ServerGame*);

        /**
         * Used to spawn another ingredient on the map randomly
         * */
        static Ingredient* spawnIngredient(GameState* gameState, Recipe* recipe);
};