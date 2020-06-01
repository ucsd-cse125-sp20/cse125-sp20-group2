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
#include <math.h>
#include <constants/gameplay_vars.h>

class ServerGame;

/**
 * This processes information from the server and modifies game state.
 * */
class GameProcessor
{
    public:
        /**
         * Initializes the lobby phase
         * */
        static void initLobbyPhase(GameState* gameState);

        /**
         * Initializes the dungeon phase
         * */
        static void initDungeonPhase(GameState* gameState, ServerGame* server);

        /**
         * Teleport players to spawn locations of the given map
         * */
        static void initPlayersLocations(Map* map, GameState* gameState);

        /*
         * Initialize the kitchen phase
         * */
        static void initKitchenPhase(GameState* gameState);

        /*
         * Initialize the ending phase
         * */
        static void initEndPhase(GameState* gameState, ServerGame* server);

        static void initDungeonWaiting(GameState* game);

        /**
         * This will process events specifically for the lobby state
         * */
        static void processLobby(unsigned int, Game::ClientMessage, ServerGame*);

        /**
         * This processes a client message for a client id.
         * This will modify GameState.
         * */
        static void process(unsigned int, Game::ClientMessage, ServerGame*);

        /**
         * Used to spawn another ingredient on the map randomly
         * */
        static Ingredient* spawnIngredient(GameState* gameState, Recipe* recipe);
};