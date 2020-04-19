#include "NetworkService.h"
#include <schema/Game.pb.h>

class NetworkClient
{
public:
    SOCKET ConnectSocket;
    NetworkClient(char* host, int port);
    ~NetworkClient();
    
    void send(Game::ClientMessage clientMessage);

    // TODO: Fix
    std::string read();

private:
    char network_data[DEFAULT_BUFLEN];
};