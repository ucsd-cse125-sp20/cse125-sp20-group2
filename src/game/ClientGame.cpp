#include <game/ClientGame.h>
#include <chrono>
#include <thread>

#define CLIENT_DELAY 

ClientGame::ClientGame(std::string IP, int port) : client(IP, port)
{
    runGame();
}

ClientGame::~ClientGame()
{

}

void ClientGame::runGame() 
{
    while(1) {
        // Send input to server
        sendMsgs();
        // Receive updated state from server
        receiveUpdates();
        // Update local game state
        updateGameState();
        // Render world
        renderWorld();
        // Sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(CLIENT_DELAY));
    }
}

void ClientGame::sendMsgs()
{
    Game::ClientMessage clientMessage;
    clientMessage.set_direction(Game::Direction::UP);
    client.send(clientMessage);
}

void ClientGame::receiveUpdates()
{
    client.read();
}

void ClientGame::updateGameState()
{
    for (Game::ServerMessage currMessage : client.messages) {
        PrintUtil::print(currMessage);
    }
}

void ClientGame::renderWorld()
{
    // TODO, FILL WITH RENDER LOOP
}