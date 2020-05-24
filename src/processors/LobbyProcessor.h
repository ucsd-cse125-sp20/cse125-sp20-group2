#include <game/GameState.h>
#include <schema/Game.pb.h>

class ServerGame;

class LobbyProcessor
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
};