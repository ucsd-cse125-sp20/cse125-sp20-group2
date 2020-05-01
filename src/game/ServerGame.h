#include <network/ServerNetwork.h>
#include <game/GameState.h>
#include <unordered_map>
#include <objects/GameObject.cpp>

class ServerGame
{
public:

    ServerGame(int port);

    ~ServerGame();

    // map of all objects in the game currently
    std::unordered_map<unsigned int, GameObject> objects;

private:
    // count of all objects
    unsigned int objNum;

    ServerNetwork server;

    GameState gameState;

    void run();

    void process();
    
    void acceptCallback(int);
};