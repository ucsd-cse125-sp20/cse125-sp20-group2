#include "NetworkService.h"
#include <string>
#include <schema/Game.pb.h>
#include <deque>

class NetworkClient
{
public:
    SOCKET ConnectSocket;
    NetworkClient(std::string host, int port);
    ~NetworkClient();
    
    void send(Game::ClientMessage clientMessage);

    // Reads server messages into a data structure
    void read();

    std::deque<Game::ServerMessage> messages;

private:
    char network_data[DEFAULT_BUFLEN];

    std::vector<char> buffer;
};