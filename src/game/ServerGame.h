#include <network/ServerNetwork.h>

class ServerGame
{
public:
    ServerGame(int port);
    ~ServerGame();

    void run();

private:
    ServerNetwork server;

};