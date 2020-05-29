#include <game/ServerGame.h>
#include <processors/GameProcessor.h>
#include <processors/LobbyProcessor.h>
#include <processors/EndProcessor.h>
#include <stdlib.h>

ServerGame::ServerGame(int port) : server(port)
{
    // Initialize the gamestate
    this->onRoundChange();
    
    // Set up function to initialize players
    std::function<void(int)> notifyClients = std::bind(&ServerGame::onClientConnect, this, std::placeholders::_1);
    this->server.setOnClientConnect(notifyClients);
    run();
}

ServerGame::~ServerGame()
{

}

int ServerGame::getTick() 
{
    return TICK;
}

void ServerGame::run()
{
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        auto msgMap = server.readAllMessages();
        
        this->preprocess(msgMap);

        this->process(msgMap);

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


void ServerGame::preprocess(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> & clientMsgs)
{
    for (auto & it : clientMsgs)
    {
        std::vector<Game::ClientMessage> & msgList = it.second;
        auto uniquePortion = std::unique(msgList.begin(), msgList.end(), google::protobuf::util::MessageDifferencer::Equals);
        msgList.erase(uniquePortion, msgList.end());
    }
}

void ServerGame::process(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> & msgMap) 
{
    /// TODO: Different ways to advance round based on the different rounds.
    if (this->gameState.gameOver()) {
        this->gameState.advanceRound();
        this->onRoundChange();
        return;
    }

    // Go through all clients
    for (auto iter = msgMap.begin(); iter != msgMap.end(); iter++) {
        
        auto clientId = iter->first;
        auto msgs = iter->second;

        for (auto msg : msgs) {

            switch (this->gameState.getRound()) {
                case Game::RoundInfo::LOBBY:
                {
                    std::cout << "This is lobby phase" << std::endl;
                    LobbyProcessor::Process(clientId, msg, this);
                    GameProcessor::Process(clientId, msg, this);
                    break;
                }
                case Game::RoundInfo::DUNGEON_WAITING:
                {
                    std::cout << "This is dungeon waiting phase" << std::endl;
                    break;
                }
                case Game::RoundInfo::DUNGEON:
                {
                    std::cout << "this is dungeon phase" << std::endl;
                    GameProcessor::Process(clientId, msg, this);
                    break;
                }
                case Game::RoundInfo::KITCHEN_WAITING:
                {
                    std::cout << "This is kitchen waiting phase" << std::endl;
                    break;
                }
                case Game::RoundInfo::KITCHEN:
                {
                    GameProcessor::Process(clientId, msg, this);
                    std::cout << "this is kitchen phase" << std::endl;
                    break;
                }
                case Game::RoundInfo::END:
                {
                    std::cout << "This is ending phase" << std::endl;
                    break;
                }
                default:
                {
                    std::cout << "Round state is not handled" << std::endl;
                }
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
    for (auto message : this->messages) {
        this->server.sendToAll(*message);
        delete message;
    }
    this->messages.clear();

    // Send player specific messages
    for (auto pair : this->specificMessages)
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
    this->specificMessages.clear();
}

void ServerGame::onClientConnect(int clientId) 
{
    // Send lobby state to the client
    Game::ServerMessage* gameStatus = MessageBuilder::toRoundUpdate(this->gameState.getRound());
    this->server.send(clientId, *gameStatus);
    std::cout << gameStatus->DebugString() << std::endl;

    // Add client to ready map
    this->gameState.readyStatus[clientId] = false;

    /// TODO: If we go image lobby route, this will not load the player or client info initially. This needs client support.
    // Add player with respective client ID
    Player* player = this->gameState.addPlayer(clientId);
   
    // while player is colliding, generate new location
    for (GameObject* obj : this->gameState.getAllObjects())
    {
        if (obj == player) continue;
        while (player->isColliding(obj))
        {
            glm::vec3 playerPos;
            playerPos.x = rand() % 6;
            playerPos.y = 0;
            playerPos.z = rand() % 6;  
            player->setPosition(playerPos);
        }
    }

    // Send objects
    for (GameObject* obj : this->gameState.getAllObjects()) {
        Game::ServerMessage* message = MessageBuilder::toServerMessage(obj);
        this->server.sendToAll(*message);
        delete message;
    }

    // Send over client info
    Game::ServerMessage* clientInfoMsg = MessageBuilder::toClientInfo(clientId, player->getID());
    this->server.send(clientId, *clientInfoMsg);
    delete clientInfoMsg;
}

void ServerGame::onRoundChange()
{
    // Send updated state to the client
    Game::ServerMessage* gameStatus = MessageBuilder::toRoundUpdate(this->gameState.getRound());
    this->server.sendToAll(*gameStatus);

    /// TODO: Handle unloading all objects. Should not unload the player object.

    // Init all objects into the world (everything preloaded)
    
    switch (this->gameState.getRound())
    {
        case Game::RoundInfo::LOBBY:
        {
            std::cout << "initializing lobby" << std::endl;
            break;
        }
        case Game::RoundInfo::DUNGEON_WAITING:
        { 
            std::cout << "Initializing dungeon waiting" << std::endl;

            // Create the other phases 
            GameProcessor::initDungeonPhase(&this->gameState);
            GameProcessor::initKitchenPhase(&this->gameState);


            this->gameState.kitchenMap->setRender(false);
            LobbyProcessor::initDungeonWaiting(&this->gameState);
            break;
        }
        case Game::RoundInfo::DUNGEON:
        {
            std::cout << "initializing dungeon" << std::endl;
            gameState.setRoundTime(Config::getInt("Dungeon_Round_Time"));
            break;
        }
        case Game::RoundInfo::KITCHEN_WAITING:
        {
            std::cout << "initializing kitchen waiting" << std::endl;
            gameState.setRoundTime(Config::getInt("Kitchen_Waiting_Round_Time"));
            this->gameState.kitchenMap->setRender(true);
            this->gameState.dungeonMap->setRender(false);
            for(auto ingredientPair : gameState.ingredientObjects )
                ingredientPair.second->setRender(false);
            // EndProcessor::initGameState(&this->gameState, this);
            break;
        }
        case Game::RoundInfo::KITCHEN:
        {
            gameState.setRoundTime(Config::getInt("Kitchen_Round_Time"));
            // "set render to false for everything not associate"
            break;
        }
        case Game::RoundInfo::END:
        {
            break;
        }
        default: 
        {
            std::cout << "Unhandled round change" << std::endl;
            break;
        }
    }

    

    // On round change, send over all objects
    for (GameObject* obj : this->gameState.getAllObjects()) {
        Game::ServerMessage* message = MessageBuilder::toServerMessage(obj);
        this->server.sendToAll(*message);
        delete message;
    }
}