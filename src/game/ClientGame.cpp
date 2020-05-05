#include <game/ClientGame.h>
#include <glm/gtx/string_cast.hpp>
#include <objects/Player.h>
#include <chrono>
#include <thread>

#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(WIN_WIDTH, WIN_HEIGHT)
{
    // TODO: fix hardcoded player values and hardcoded window insertion
    GameObject* grid = new GameObject(999999);
    grid->setPosition(glm::vec3(0, -1, 0));
    grid->setModel("assets/models/grid_square.obj");
    window.addObject(999999, grid);

    GameObject* tomato = new GameObject(2);
    tomato->setPosition(glm::vec3(0, 0.5, -2));
    tomato->setModel("assets/models/tomato.obj");
    window.addObject(2, tomato);

    GameObject* lettuce = new GameObject(3);
    lettuce->setPosition(glm::vec3(-1, 0.5, -2));
    lettuce->setModel("assets/models/Lettuce.obj");
    window.addObject(3, lettuce);

    GameObject* cheese = new GameObject(4);
    cheese->setPosition(glm::vec3(1, 0.5, -2));
    cheese->setModel("assets/models/Cheese.obj");
    window.addObject(4, cheese);

    GameObject* onion = new GameObject(5);
    onion->setPosition(glm::vec3(2, 0.5, -2));
    onion->setModel("assets/models/Onion.obj");
    window.addObject(5, onion);

    GameObject* meat = new GameObject(6);
    meat->setPosition(glm::vec3(-2, 0.5, -2));
    meat->setModel("assets/models/meat.obj");
    window.addObject(6, meat);

    GameObject* nanosuit = new GameObject(7);
    nanosuit->setPosition(glm::vec3(0, -2, -6));
    nanosuit->setModel("assets/nanosuit/nanosuit.obj");
    window.addObject(7, nanosuit);

    runGame();
}

ClientGame::~ClientGame()
{

}

void ClientGame::runGame() 
{
    while(!window.isClosed) {
        
        // Take local input
        // Send to the server
        processInput();

        // may not need this
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

        if (window.objectsToRender.count(id) > 0) {
            GameObject* obj = window.objectsToRender[id];
            obj->setRotation(rotation);
            obj->setPosition(glm::vec3(location.x(), location.y(), location.z()));
        } else {
            // Insert object into window
            GameObject* newGameObject = new GameObject(id);
            newGameObject->setModel("assets/models/cube.obj");   // TODO: either read from server or config file which model to use
            window.addObject(id, newGameObject);
        }
        
        PrintUtil::print(currMessage);
    }
    client.messages.clear();
}

/**
 * Take in input from the viewport window.
 * */
void ClientGame::processInput()
{
	// Exit application.
	if (glfwGetKey(window.glfwViewport, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window.glfwViewport, true);
	}

    // Message to send
    Game::ClientMessage msg;

    // Get key inputs and set direction of message
	if (glfwGetKey(window.glfwViewport, GLFW_KEY_W) == GLFW_PRESS)
    {
        msg.set_direction(Game::Direction::UP);  
    }
	else if (glfwGetKey(window.glfwViewport, GLFW_KEY_S) == GLFW_PRESS) 
    {
        msg.set_direction(Game::Direction::DOWN); 
    }
	if (glfwGetKey(window.glfwViewport, GLFW_KEY_A) == GLFW_PRESS)
    {
        msg.set_direction(Game::Direction::LEFT); 
    }
	else if (glfwGetKey(window.glfwViewport, GLFW_KEY_D) == GLFW_PRESS)
    {
        msg.set_direction(Game::Direction::RIGHT); 
    }
    
    // Send message only if it has a direction associated with it
    if (msg.has_direction()) {
        this->client.send(msg);
    }
}