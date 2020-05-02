#include <network/NetworkService.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <thread>
#include <functional>
#include <schema/Game.pb.h>

class ServerNetwork
{
public:

    /**
     * Initializes the ServerNetwork. This will setup listening to the respective port.
     * These sockets are non-blocking.
     * @param port the port to bind to
     * */
    ServerNetwork(int port);

    ~ServerNetwork();
    
    /**
     * Sends a server message to a client. 
     * @param clientId The client to send a message to
     * @param msg The message to send
     * */
    void send(unsigned int clientId, Game::ServerMessage msg);

    /**
     * Sends a server message to all clients.
     * 
     * @param msg The message to send
     * */
    void sendToAll(Game::ServerMessage msg);

    /**
     * Reads all messages queued in the socket.
     * 
     * @returns a map of clientId to client messages. 
     * */
    std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> readAllMessages();

    void setAcceptCallback(std::function<void(int)> func);

private:
    /**
     * The socket the server binds to.
     * */
    SOCKET listenSocket;

    /**
     * The thread the server uses to accept new clients.
     * */ 
    std::thread threadAccept;

    /**
     * This holds temporary data while reading from the socket.
     * */
    char network_data[DEFAULT_BUFLEN];
    
    /**
     * Next id to assign to client.
     * */
    unsigned int nextId = 0;

    /**
     * This is the list of users who are connected.
     * This is a mapping of clientId to their respective socket.
     * */
    std::map<unsigned int, SOCKET> sessions;

    /**
     * This holds temporary data while reading from the socket. 
     * After being copied to buffer from the respective socket, we use a temporary buffer to queue up data. 
     * This handles cases where we receive LESS than the expected size.
     * */
    std::map<unsigned int, std::vector<char>> buffers;

    unsigned int getNextId();

    /**
     * Creates a new socket for any connecting clients, and
     * adds them to "sessions" under its client id.
     * */
    void startAccepting();

    /**
     * This function is called when a new client is accepted. 
     * Calls the function with the incoming clientId.
     * IMPORTANT: This is called on another thread.
     * */
    std::function<void(int)> acceptCallback;
    
};