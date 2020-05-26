#include <schema/Game.pb.h>
#include <game/ServerGame.h>

class EventProcessor
{
    public:
        virtual void Process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame* server) = 0;


    private:
};