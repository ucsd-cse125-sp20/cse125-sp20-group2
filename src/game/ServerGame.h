#include <network/ServerNetwork.h>
#include <game/GameState.h>

class ServerGame
{
public:
    ServerGame(int port);
    ~ServerGame();


private:
    ServerNetwork server;

    GameState gameState;

    void run();

    void process();
    
    void acceptCallback(int);
};