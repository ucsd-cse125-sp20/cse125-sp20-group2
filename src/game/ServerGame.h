#include <network/ServerNetwork.h>
#include <game/GameState.h>
#include <unordered_map>
#include <objects/GameObject.cpp>

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