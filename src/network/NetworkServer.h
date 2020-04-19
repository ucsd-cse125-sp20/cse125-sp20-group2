#include "NetworkService.h"
#include <map>
#include <unordered_map>
#include <vector>
#include <schema/Game.pb.h>

class ServerNetwork
{
public:
    ServerNetwork(int port);
    ~ServerNetwork();
    
    void send(unsigned int clientID, Game::ServerMessage msg);

    void sendToAll(Game::ServerMessage msg);

    std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> readAllMessages();
private:
    SOCKET listenSocket;

    std::thread threadAccept;

    char network_data[DEFAULT_BUFLEN];

    unsigned int nextId = 0;

    std::map<unsigned int, SOCKET> sessions;

    std::map<unsigned int, std::vector<char>> buffers;

    unsigned int getNextId();

    void startAccepting();

};