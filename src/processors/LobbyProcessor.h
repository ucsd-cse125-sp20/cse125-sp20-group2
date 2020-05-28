#include <game/GameState.h>
#include <schema/Game.pb.h>
#include <game/ServerGame.h>
#include <util/Config.h>

class ServerGame;

class LobbyProcessor
{
    public:
        /**
         * Initializes lobby phase
         * */
        static void initLobby(GameState* gameState);

        /**
         * Initialize the dungeon waiting phase
         * */
        static void initDungeonWaiting(GameState* gameState);

        /**
         * This processes a client message for a client id.
         * This will modify GameState.
         * */
        static void Process(unsigned int, Game::ClientMessage, ServerGame*);    

        
};