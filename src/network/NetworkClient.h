#include <network/NetworkService.h>
#include <string>
#include <schema/Game.pb.h>
#include <deque>

class NetworkClient
{
public: 
    /**
     * Socket used by this client to connect to the server.
     * These sockets are non-blocking.
     * */
    SOCKET ConnectSocket;

    /**
     * Initializes a NetworkClient w/ host and port
     * @param host IP of the server
     * @param port port of the server
     * */
    NetworkClient(std::string host, int port);

    ~NetworkClient();

    /**
     * Sends a message to the server
     * @param clientMessage An instance of a ClientMessage from protobuf
     * */
    void send(Game::ClientMessage clientMessage);

    /**
     * Reads all messages from server into messages data structure
     * */
    void read();

    /**
     * Queue used to hold all ServerMessages from the server
     * */
    std::deque<Game::ServerMessage> messages;
private:
    /** 
     * Used to temporarily store incoming data from server
     * */
    char network_data[DEFAULT_BUFLEN];

    /**
     * Used to queue up chars from server, to be converted to objects
     * */
    std::vector<char> buffer;
};