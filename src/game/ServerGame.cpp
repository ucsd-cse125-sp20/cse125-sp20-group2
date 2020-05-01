#include <game/ServerGame.h>
#include <util/PrintUtil.h>
#include <util/MessageBuilder.h>
#include <thread>
#include <chrono>

#define TICK 1000

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
            std::cout << "Time to process: " << dur.count() << " Sleeping for: " << sleepTime.count() << std::endl;
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
            PrintUtil::print(msg);

            Game::ServerMessage* message = this->processor.messages.front();
            this->processor.messages.pop_front();
            this->server.sendToAll(*message);
            free(message);

            std::cout << "I hate c memory" << std::endl;
        }
        
    }
}

// ASSUMES THERE IS ONLY ONE PLAYER
void ServerGame::acceptCallback(int clientId) 
{
    int objId = this->gameState.addPlayer(clientId);
    GameObject* object = this->gameState.getGameObject(objId);
    std::cout << "i hate this" << std::endl;
    Game::ServerMessage* message = MessageBuilder::toServerMessage(object);
    this->server.sendToAll(*message);
    std::cout << "Gonna free xd" << std::endl;
    free(message);
    std::cout << "After free xd" << std::endl;
    std::cout << "Returning from accept callback" << std::endl;

    // OK, THIS COVERS SENDING TO ALL. 
    // WE NEED TO COVER THE LIST OF OBJECTS EXISTING
    // SHOULD BE SIMPLE RIGHT
}