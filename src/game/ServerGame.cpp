#include <game/ServerGame.h>

#define TICK 30

ServerGame::ServerGame(int port) : server(port), processor(&this->gameState)
{
    // std::function<void(int)> test = std::bind(&ServerGame::acall, this)
    std::function<void(int)> notifyClients = std::bind(&ServerGame::onClientConnect, this, std::placeholders::_1);
    this->server.setOnClientConnect(notifyClients);
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
    this->processor.Preprocess(map);

    // Go through all clients
    for (auto iter = map.begin(); iter != map.end(); iter++) {
        auto clientId = iter->first;
        auto msgs = iter->second;
        for (auto msg : msgs) {
            // PrintUtil::print(msg);
            this->processor.Process(clientId, msg, TICK);
            
            if (this->processor.messages.size() > 0) {
                Game::ServerMessage* message = this->processor.messages.front();
                this->processor.messages.pop_front();
                this->server.sendToAll(*message);
                delete message;
            }

            if (this->processor.specificMessages[clientId].size() > 0) {
                Game::ServerMessage* message = this->processor.specificMessages[clientId].front();
                this->processor.specificMessages[clientId].pop_front();
                this->server.send(clientId, *message);
                delete message;
            }
        }
    }

    /// TODO: mainly for testing, timer functionality, can delete
    if (this->gameState.timeHasUpdated())
    {
        // Create time update message
        Game::TimeUpdate* timeUpdateMessage = new Game::TimeUpdate();
        timeUpdateMessage->set_seconds(this->gameState.getRoundTime());
        Game::ServerMessage* serverMsg = new Game::ServerMessage();
        serverMsg->set_allocated_time(timeUpdateMessage);

        // Send round update to everyone
        this->server.sendToAll(*serverMsg);

        delete serverMsg;
    }

    if (this->gameState.gameOver())
    {
        std::cout << "GAME OVER AAAAAAAAAAAAAA" << std::endl;
    }
}

// Only called from server network when it accepts a new client
void ServerGame::onClientConnect(int clientId) 
{
    // // Add player with respective client ID
    // this->gameState.addPlayer(clientId);

    // // Grab player object
    // GameObject* playerObject = this->gameState.getPlayerObject(clientId);

    // // Build a message
    // Game::ServerMessage* createPlayerMessage = MessageBuilder::toServerMessage(playerObject);

    // // Send out, then free
    // this->server.sendToAll(*createPlayerMessage);
    // delete createPlayerMessage;

    // // Send over client info
    // Game::ServerMessage* clientInfoMsg = MessageBuilder::toClientInfo(clientId, playerObject->getID());
    // this->server.send(clientId, *clientInfoMsg);
    // delete clientInfoMsg;

    // // First, send all game objects
    // for (auto objectPair : this->gameState.getObjects())
    // {
    //     auto objectPtr = objectPair.second;
    //     Game::ServerMessage* message = MessageBuilder::toServerMessage(objectPtr);
    //     this->server.sendToAll(*message);
    //     delete message;
    // }

    // // Then, send all ingredients on the map (if applicable)
    // for (auto ingredientObjectPair : this->gameState.getIngredientObjects())
    // {
    //     auto ingredientPtr = ingredientObjectPair.second;
    //     Game::ServerMessage* message = MessageBuilder::toServerMessage(ingredientPtr);
    //     this->server.sendToAll(*message);
    //     delete message;
    // }

    // // Next, send all existing players
    // for (auto playerObjectPair : this->gameState.getPlayerObjects())
    // {
    //     auto playerPtr = playerObjectPair.second;
    //     Game::ServerMessage* message = MessageBuilder::toServerMessage(playerPtr);
    //     this->server.sendToAll(*message);
    //     delete message;
    // }
}

// OnClientConnect: Send a lobby state.
// onRoundChange: 
//  switch: Lobby, Send a lobby message. 
// Prepare for next phase