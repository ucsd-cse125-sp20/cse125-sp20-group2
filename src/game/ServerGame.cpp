#include <game/ServerGame.h>
#include <util/PrintUtil.h>
#include <util/MessageBuilder.h>
#include <thread>
#include <chrono>

#define TICK 30

ServerGame::ServerGame(int port) : server(port), processor(&this->gameState)
{
    // std::function<void(int)> test = std::bind(&ServerGame::acall, this)
    std::function<void(int)> notifyClients = std::bind(&ServerGame::acceptCallback, this, std::placeholders::_1);
    this->server.setAcceptCallback(notifyClients);

    run();
}

ServerGame::~ServerGame()
{

}

void ServerGame::run()
{
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        this->process();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dur = end - start;

        std::chrono::duration<double> sleepTime = std::chrono::milliseconds(TICK) - dur;

        if (sleepTime.count() >= 0) {
            // std::cout << "Time to process: " << dur.count() << " Sleeping for: " << sleepTime.count() << std::endl;
            std::this_thread::sleep_for(sleepTime);
        } else {
            std::cout << "Server is behind by: " << dur.count() << " no panic " << std::endl;
        }
    }
}

void ServerGame::process() 
{
    auto map = server.readAllMessages();
    
    // preprocess and remove all stuff here

    // Go through all clients
    // This is just an example. This isn't necessarily the correct logic.
    for (auto iter = map.begin(); iter != map.end(); iter++) {
        auto clientId = iter->first;
        auto msgs = iter->second;
        for (auto msg: msgs) {
            // PrintUtil::print(msg);
            this->processor.process(clientId, msg, TICK);
            
            if (this->processor.messages.size() > 0) {
                Game::ServerMessage* message = this->processor.messages.front();
                this->processor.messages.pop_front();
                this->server.sendToAll(*message);
                delete message;
                // free(message);
                break; // TODO: Remove, this preprocesses messages s.t. only one is from each player per tick
            }

            if (this->processor.specificMessages[clientId].size() > 0) {
                Game::ServerMessage* message = this->processor.specificMessages[clientId].front();
                this->processor.specificMessages[clientId].pop_front();
                this->server.send(clientId, *message);
                delete message;
            }
        }
    }
}

// Only called from server network when it accepts a new client
void ServerGame::acceptCallback(int clientId) 
{
    int clientIdReturn = this->gameState.addPlayer(clientId);

    // Grab player object
    GameObject* playerObject = this->gameState.getPlayerObject(clientIdReturn);

    // Build a message
    Game::ServerMessage* message = MessageBuilder::toServerMessage(playerObject);

    // Send out, then free
    this->server.sendToAll(*message);
    // free(message);
    delete message;

    // OLD CODE
    // int objId = this->gameState.addPlayer(clientId);
    // GameObject* object = this->gameState.getGameObject(objId);
    // Game::ServerMessage* message = MessageBuilder::toServerMessage(object);
    // this->server.sendToAll(*message);
    // free(message);

    // OK, THIS COVERS SENDING TO ALL. 
    // WE NEED TO COVER THE LIST OF OBJECTS EXISTING
    // SHOULD BE SIMPLE RIGHT

    // First, send all game objects
    for (auto objectPair : this->gameState.getObjects())
    {
        auto objectPtr = objectPair.second;
        Game::ServerMessage* message = MessageBuilder::toServerMessage(objectPtr);
        this->server.sendToAll(*message);
        // free(message);
        delete message;
    }

    // Then, send all ingredients on the map (if applicable)
    for (auto ingredientObjectPair : this->gameState.getIngredientObjects())
    {
        auto ingredientPtr = ingredientObjectPair.second;
        Game::ServerMessage* message = MessageBuilder::toServerMessage(ingredientPtr);
        this->server.sendToAll(*message);
        // free(message);
        delete message;
    }

    // Next, send all existing players
    for (auto playerObjectPair : this->gameState.getPlayerObjects())
    {
        auto playerPtr = playerObjectPair.second;
        Game::ServerMessage* message = MessageBuilder::toServerMessage(playerPtr);
        this->server.sendToAll(*message);
        // free(message);
        delete message;
    }
}