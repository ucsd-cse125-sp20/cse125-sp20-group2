#include <game/ClientGame.h>
#include <game/KeyResolver.h>

#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(Config::getFloat("Window_Width"), Config::getFloat("Window_Height"))
{
    // Configure keybinds
    glfwSetWindowUserPointer(this->window.glfwViewport, reinterpret_cast<void*> (this));
    glfwSetKeyCallback(this->window.glfwViewport, key_callback_wrapper);

    runGame();
}

ClientGame::~ClientGame()
{

}

void ClientGame::keyBindsHandler(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
    // Handles ready up
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        std::cout << "sending ready up message" << std::endl;
        Game::ClientMessage* readyMsg = MessageBuilder::toReadyMessage(true);
        this->client.send(*readyMsg);
        delete readyMsg;
    }

    // Handles toggling free cam
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        std::cout << "locking / unlocking the camera" << std::endl;
        this->window.camera->toggleFreeCam();
    }
}

void ClientGame::runGame() 
{
    while(!window.isClosed) 
    {
        // Take local input
        // Send to the server
        processInput();

        /// TODO: SEND PENDING MSG LATER IF YOU WANT TO QUEUE UP
        // sendPendingMsgs()

        // Receive updated state from server
        receiveUpdates();

        // Update local game state
        updateGameState();

        // Render world
        window.render();
    }
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
                bool render = currMessage.object().render();

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
                obj->setRender(render);
                obj->setPosition(glm::vec3(location.x(), location.y(), location.z()));
                break;
            }

            case Game::ServerMessage::EventCase::kScore:
            {
                window.setScore(currMessage.score().currscore());
                break;
            }

            // Inventory update messages
            case Game::ServerMessage::EventCase::kInventory: 
            {
                // Get player associated with this client
                Player* player = (Player*)window.objectsToRender[objectId];

                // Get id of the object to be picked up.
                IngredientObject* pickup = (IngredientObject*)window.objectsToRender[currMessage.inventory().id()];

                pickup->setName(currMessage.inventory().name());

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
                window.camera->setTarget(window.objectsToRender[this->objectId]);
                Player* p = (Player*) window.objectsToRender[this->objectId];
                window.addInventory(p->getInventory());
                break;
            }

            case Game::ServerMessage::EventCase::kTime:
            {
                
                std::cout << "secs left " << currMessage.time().seconds() << std::endl;
                uint32_t seconds = currMessage.time().seconds();
                if( seconds == 0) {
                    this->round++;
                    window.setRound(this->round);
                }
                window.setTimer(seconds);
                break;
            }

            case Game::ServerMessage::EventCase::kRound:
            {
                std::cout << " received round update message " << currMessage.DebugString() << std::endl;
                break;
            }

            /// TODO: NEED TO CHANGE LATER
            case Game::ServerMessage::EventCase::kWin:
            {
                std::cout << " received win event " << std::endl;
                window.gameOver = true;

                // Win or lose
                if (currMessage.win().clientid() == clientId)
                    window.gameWin = true;
                else
                    window.gameWin = false;

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
    
    // Send message only if it has a direction associated with it
    if (msg.has_direction()) {
        this->client.send(msg);
    }
}