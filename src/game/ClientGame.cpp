#include <game/ClientGame.h>


#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(Config::getFloat("Window_Width"), Config::getFloat("Window_Height"))
{
    /// TODO: fix hardcoded player values and hardcoded window insertion
    GameObject* grid = new GameObject(999999);
    grid->setPosition(glm::vec3(0, 0, 0));
    grid->setModel(Config::get("Maze_Model"));
    grid->applyScale(glm::vec3(2, 2, 2));
    window.addObject(999999, grid);

    /*Map* m = MapBuilder::getBasicMap();
    for(auto it = m->wallList.begin(); it!= m->wallList.end(); it++) {
        window.addObject((*it)->getID(), *it);
    }

    for(auto it = m->ingredients.begin(); it != m->ingredients.end(); it++) {
        window.addObject((*it)->getID(), *it);
    }*/

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


    /// TODO: Assume only object for now. Update GameObject to respective type
    /// TODO: switch case based on what kind of messages
    for (Game::ServerMessage currMessage : client.messages) {

        // switch(currMessage.event_case()) {
        //     case Game::ServerMessage::EventCase::kInventory:
        //     {

        //     }
        // }
        Game::Vector3 location = currMessage.object().worldposition();
        float rotation = currMessage.object().rotation();
        uint32_t id = currMessage.object().id();

        GameObject* obj = NULL;
        // Update object state
        if (window.objectsToRender.count(id) > 0) {
            obj = window.objectsToRender[id];
        } else {
            /// Insert object into window (TODO: instantiate player object)
            obj = new GameObject(id);
            obj->setModel(Config::get("Character_Model"));
            window.addObject(id, obj);

            /// TODO: Hardcoded camera target set
            window.camera->setTarget(obj);
        }
        obj->setRotation(rotation);
        obj->setPosition(glm::vec3(location.x(), location.y(), location.z()));
        
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

    // Camera movement options (client-side only)
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_UP) == GLFW_PRESS)
		window.camera->processKeyMovement(FORWARD);
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_DOWN) == GLFW_PRESS)
		window.camera->processKeyMovement(BACKWARD);
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_LEFT) == GLFW_PRESS)
		window.camera->processKeyMovement(LEFT);
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_RIGHT) == GLFW_PRESS)
		window.camera->processKeyMovement(RIGHT);
	if (glfwGetKey(window.glfwViewport, GLFW_KEY_F) == GLFW_PRESS)
		window.camera->toggleFreeCam();

    
    // Send message only if it has a direction associated with it
    if (msg.has_direction()) {
        this->client.send(msg);
    }
}