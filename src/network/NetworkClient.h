#include "NetworkService.h"
/**
 * connect
 * is_connected
 * send
 * read
 * client id 
 */

class NetworkClient
{
public:
    SOCKET ConnectSocket;
    NetworkClient(char* host, int port);
    ~NetworkClient();
    
    // send();

    // read();

private:
    char* host;

    int port;

    void setup();

    char network_data[DEFAULT_BUFLEN];
};