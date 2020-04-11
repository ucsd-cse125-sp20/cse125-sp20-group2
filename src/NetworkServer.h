#include "NetworkService.h"
#include <map>

class ServerNetwork
{
public:

    ServerNetwork(int port);
    ~ServerNetwork();
    
    // send();

    // sendToAll();

private:
    SOCKET ListenSocket;

    char network_data[DEFAULT_BUFLEN];

    int port;
    
    int next_id = 0;

    void setup();

    std::map<unsigned int, SOCKET> sessions;

    int getNextID();

    void startAccepting();

    void readAllMessages();
};