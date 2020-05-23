#include <game/ServerGame.h>

#define TICK 30

ServerGame::ServerGame(int port) : server(port), gameProcessor(&this->gameState)
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

        this->sendPendingMessages();

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
    this->gameProcessor.Preprocess(map);

    // Go through all clients
    for (auto iter = map.begin(); iter != map.end(); iter++) {
        
        auto clientId = iter->first;
        auto msgs = iter->second;

        switch (this->gameState.getRound()) {
            case Game::RoundInfo::LOBBY:
            {
                this->updateLobbyState(clientId, msgs);
                break;
            }
            case Game::RoundInfo::DUNGEON:
            case Game::RoundInfo::KITCHEN:
            {
                this->updatePlayingState(clientId, msgs);
                break;
            }
            case Game::RoundInfo::DUNGEON_WAITING:
            case Game::RoundInfo::KITCHEN_WAITING:
            {
                this->updateWaitingState(clientId, msgs);
                break;
            }
            case Game::RoundInfo::END:
            {
                break;
            }
            default:
            {
                std::cout << "Round state is not handled" << std::endl;
            }
        }
    }

    /// TODO: Remove
    // /// TODO: mainly for testing, timer functionality, can delete
    // if (this->gameState.timeHasUpdated())
    // {
    //     // Create time update message
    //     Game::TimeUpdate* timeUpdateMessage = new Game::TimeUpdate();
    //     timeUpdateMessage->set_seconds(this->gameState.getRoundTime());
    //     Game::ServerMessage* serverMsg = new Game::ServerMessage();
    //     serverMsg->set_allocated_time(timeUpdateMessage);

    //     // Send round update to everyone
    //     this->server.sendToAll(*serverMsg);

    //     delete serverMsg;
    // }

    // /// TODO FIX LOGIC
    // if (this->gameState.gameOver())
    // {
    //     // std::cout << "GAME OVER AAAAAAAAAAAAAA" << std::endl;
    // }
}

void ServerGame::sendPendingMessages()
{
    // Send global messages
    for (auto message : this->gameProcessor.messages) {
        this->server.sendToAll(*message);
        delete message;
    }
    this->gameProcessor.messages.clear();

    // Send player specific messages
    for (auto pair : this->gameProcessor.specificMessages)
    {
        auto clientId = pair.first;
        auto messages = pair.second;

        for (auto msg : messages)
        {
            this->server.send(clientId, *msg);
            delete msg;
        }
        messages.clear();
    }
}

void ServerGame::updateLobbyState(unsigned int clientId, std::vector<Game::ClientMessage> msgs)
{
    // this->lobbyProcessor.process()
    // this->gameProcessor.Process
    // 
    // start a timer when all players are ready.
    // movement event = ready
}

void ServerGame::updateWaitingState(unsigned int clientId, std::vector<Game::ClientMessage> msgs)
{

}

void ServerGame::updatePlayingState(unsigned int clientId, std::vector<Game::ClientMessage> msgs)
{
    for (auto msg: msgs) this->gameProcessor.Process(clientId, msg, TICK);
}

void ServerGame::onClientConnect(int clientId) 
{
    // Send lobby state to the client
    Game::ServerMessage* gameStatus = MessageBuilder::toRoundUpdate(this->gameState.getRound());
    this->server.sendToAll(*gameStatus);

    /// TODO: If we go image lobby route, this will not load the player or client info initially. This needs client support.
    // Add player with respective client ID
    Player* player = this->gameState.addPlayer(clientId);

    for (GameObject* obj: this->gameState.getAllObjects()) {
        Game::ServerMessage* message = MessageBuilder::toServerMessage(obj);
        this->server.sendToAll(*message);
        delete message;
    }

    // Send over client info
    Game::ServerMessage* clientInfoMsg = MessageBuilder::toClientInfo(clientId, player->getID());
    this->server.send(clientId, *clientInfoMsg);
    delete clientInfoMsg;
}
