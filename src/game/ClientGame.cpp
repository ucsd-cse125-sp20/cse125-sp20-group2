#include <game/ClientGame.h>


#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(Config::getFloat("Window_Width"), Config::getFloat("Window_Height"))
{
    /// TODO: fix hardcoded player values and hardcoded window insertion
    /*GameObject* grid = new GameObject(999999);
    grid->setPosition(glm::vec3(0, 0, 0));
    grid->setModel(Config::get("Maze_Model"));
    grid->applyScale(glm::vec3(2, 2, 2));
    window.addObject(999999, grid);*/

    /// TODO: Remove later. Using currently for example usage.
    /*GameObject* nanosuit = new GameObject(3946);
    nanosuit->setModel("assets/nanosuit/nanosuit.obj");
    nanosuit->applyScale(glm::vec3(0.2, 0.2, 0.2));
    window.addObject(3946, nanosuit);*/

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
    // Process messages to update client game state
    for (Game::ServerMessage currMessage : client.messages)
    {
        // Process different types of messages
        switch(currMessage.event_case()) 
        {
            // Object-related messages
            case Game::ServerMessage::EventCase::kObject:
            {
                Game::Vector3 location = currMessage.object().worldposition();
                float rotation = currMessage.object().rotation();
                uint32_t id = currMessage.object().id();

                GameObject* obj = NULL;

                // Update existing object state
                if (window.objectsToRender.count(id) > 0) 
                {
                    obj = window.objectsToRender[id];
                }

                // Instantiate new object 
                else 
                {
                    // Different object types
                    switch(currMessage.object().type())
                    {
                        // Player object.
                        case Game::PLAYER: obj = new Player(id); break;

                        // Ingredient object.
                        case Game::INGREDIENT: obj = new IngredientObject(id); break;

                        // Cookware object.
                        case Game::COOKWARE: obj = new CookwareObject(id); break;

                        // Wall object.
                        case Game::WALL: obj = new Wall(id); break;

                        // Basic gameobject.
                        default: obj = new GameObject(id); break;
                    }

                    /// Set model based on the model path provided by the server TODO: Might not be necessary???
                    obj->setModel(currMessage.object().modelpath());

                    // Add object to window
                    window.addObject(id, obj);
                }

                // Set object parameters
                obj->setRotation(rotation);
                obj->setPosition(glm::vec3(location.x(), location.y(), location.z()));
                break;
            }

            ///TODO: Score update messages
            case Game::ServerMessage::EventCase::kScore:
            {
                break;
            }

            /// Inventory update messages (TODO: Testing required.)
            case Game::ServerMessage::EventCase::kInventory: 
            {
                // Get player associated with this client
                Player* player = (Player*)window.objectsToRender[objectId];

                // Get id of the object to be picked up.
                IngredientObject* pickup = (IngredientObject*)window.objectsToRender[currMessage.inventory().id()];

                // Add object to player inventory.
                if (currMessage.inventory().add())  player->addToInventory(pickup);
                else    player->removeFromInventory(pickup);
                std::cout << "Picked up: " << pickup->getName() << std::endl;

                break;
            }

            // Client info messages
            case Game::ServerMessage::EventCase::kClientInfo:
            {
                // Set client id
                this->clientId = currMessage.clientinfo().clientid();
                std::cout << "Connected. Client ID: " << clientId << std::endl;

                // Set object id
                this->objectId = currMessage.clientinfo().objectid();

                // Set camera target
                window.camera->setTarget(window.objectsToRender[currMessage.clientinfo().objectid()]);
                break;
            }

            // All other messages
            default:
            {
                std::cerr << "Unidentified message type." << std::endl; 
            }
        }
        //PrintUtil::print(currMessage);
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