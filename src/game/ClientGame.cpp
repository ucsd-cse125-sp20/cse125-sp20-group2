#include <game/ClientGame.h>
#include <objects/Player.h>
#include <chrono>
#include <thread>

#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(WIN_WIDTH, WIN_HEIGHT)
{
    // TODO: fix hardcoded player values and hardcoded window insertion
    GameObject* grid = new GameObject(999999);
    grid->moveTo(glm::vec3(0, -1, 0));
    grid->setModel("assets\\models\\grid_square.obj");
    window.addObject(999999, grid);

    runGame();
}

ClientGame::~ClientGame()
{

}

void ClientGame::runGame() 
{
    while(!window.isClosed) {
        // Take local input
        window.processInput();

        // Send input to server
        //sendMsgs();

        // Receive updated state from server
        receiveUpdates();

        // Update local game state
        updateGameState();

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
    // TODO: Assume only object for now. Update GameObject to respective type
    for (Game::ServerMessage currMessage : client.messages) {

        // Convert into object
        Game::Vector3 location = currMessage.object().worldposition();
        float rotation = currMessage.object().rotation();
        uint32_t id = currMessage.object().id();
        Game::ObjectType type = currMessage.object().type();

        // Insert object into window
        GameObject* newGameObject = new GameObject(id);
        newGameObject->setModel("assets\\models\\cube.obj");   // TODO: either read from server or config file which model to use
        window.addObject(id, newGameObject);

        
        PrintUtil::print(currMessage);
    }
    client.messages.clear();
}

// FIXME - SHOULD SEND MESSAGES TO SERVER BEFORE DOING THIS
void ClientGame::processInput()
{
    //if (GetAsyncKeyState(0x57)) std::cerr << "W" <<std::endl;
}