#include <game/ClientGame.h>
#include <objects/Player.h>
#include <chrono>
#include <thread>

#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(WIN_WIDTH, WIN_HEIGHT), clientID(0)
{
    // TODO: fix hardcoded player values and hardcoded window insertion
    
    Player player = Player("C:\\Users\\JQ124\\Desktop\\CompSci\\cse125\\cse125-sp20-group2\\assets\\models\\cube.obj", glm::vec3(0, -1, 0), 0.2, 0); // FIXME FIXME TODO HARDCODED CLIENTID
    window.addObject(1, player);

    runGame();
}

ClientGame::~ClientGame()
{

}

void ClientGame::runGame() 
{
    while(!window.isClosed) {
        // Take local input


        // Send input to server
        //sendMsgs();
        // Receive updated state from server
        //receiveUpdates();
        // Update local game state
        //updateGameState();

        // Render world
        window.render();

        // Sleep
        //std::this_thread::sleep_for(std::chrono::milliseconds(CLIENT_DELAY));
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
    // while (!client.messages.empty()) {
    //     PrintUtil::print(client.messages.front());
    //     client.messages.pop_front();
    // }
    for (Game::ServerMessage currMessage : client.messages) {
        PrintUtil::print(currMessage);
    }
    client.messages.clear();
}

// FIXME - SHOULD SEND MESSAGES TO SERVER BEFORE DOING THIS
void ClientGame::processInput()
{
    //if (GetAsyncKeyState())
}