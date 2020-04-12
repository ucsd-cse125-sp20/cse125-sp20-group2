#include "NetworkService.h"
#include <map>
#include <unordered_map>
#include <vector>

class ServerNetwork
{
public:

    ServerNetwork(int port);
    ~ServerNetwork();
    
    void send(unsigned int clientID, std::string msg);

    void sendToAll(std::string msg);

    std::unordered_map<unsigned int, std::vector<std::string>> readAllMessages();
private:
    SOCKET listenSocket;

    std::thread threadAccept;

    char network_data[DEFAULT_BUFLEN];

    unsigned int nextID = 0;

    std::map<unsigned int, SOCKET> sessions;

    unsigned int getNextID();

    void startAccepting();

};