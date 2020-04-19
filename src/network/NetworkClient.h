#include "NetworkService.h"
#include <schema/Game.pb.h>

class NetworkClient
{
public:
    SOCKET ConnectSocket;
    NetworkClient(char* host, int port);
    ~NetworkClient();
    
    void send(Game::ClientMessage clientMessage);

    // Reads server messages into a data structure
    // TODO: may want to queue up reads
    void read();

    std::vector<Game::ServerMessage> messages;

private:
    char network_data[DEFAULT_BUFLEN];

    std::vector<char> buffer;
};