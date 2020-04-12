#include "NetworkService.h"

class NetworkClient
{
public:
    SOCKET ConnectSocket;
    NetworkClient(char* host, int port);
    ~NetworkClient();
    
    void send(std::string);

    std::string read();

private:
    char network_data[DEFAULT_BUFLEN];
};