#include <game/GameState.h>
#include <schema/Game.pb.h>
#include <game/ServerGame.h>

class ServerGame;

class EndProcessor
{
    public:
        /**
         * Initializes the game state
         * */
        static void initGameState(GameState* gameState, ServerGame* server);
};