#include <game/ClientGame.h>
#include <game/KeyResolver.h>
#include <objects/Player.h>
#include <objects/Plate.h>

#define CLIENT_DELAY 1000

ClientGame::ClientGame(std::string IP, int port) : client(IP, port), window(Config::getFloat("Window_Width"), Config::getFloat("Window_Height"))
{
    // Configure keybinds
    glfwSetWindowUserPointer(this->window.glfwViewport, reinterpret_cast<void*> (this));
    glfwSetKeyCallback(this->window.glfwViewport, key_callback_wrapper);

    // Create floor lol
    GameObject* floor = new GameObject(-1);
    floor->setModel(Config::get("Floor_Model"));
    floor->setPosition(glm::vec3(0,-0.5,0));
    floor->applyScale(glm::vec3(2));
    window.addObject(-1, floor);

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
        this->window.toggleCursor();
    }

    // Handles interact event
    if (key == GLFW_KEY_E && action == GLFW_PRESS && this->window.getSelectedIngredient() != NULL && this->window.getRound() == KITCHEN_NUM )
    {
        std::cout << "pressed interact key" << std::endl;
        Game::ClientMessage* cookMsg = MessageBuilder::toCookMessage(this->window.getSelectedIngredient());
        this->client.send(*cookMsg);
        delete cookMsg;

        // Prevent spamming the event key, have to select ingredient again
        this->window.selectedIngredient = NULL;
    }
}

void ClientGame::runGame() 
{
    // Background music
    music.openFromFile("assets/audio/8Bit_Paradise.ogg");
    music.setVolume(Config::getInt("Background_Music_Volume"));
    music.setLoop(true);
    music.play();

    while(!window.isClosed) 
    {
        // Take local input
        // Send to the server
        processInput();

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
    // Assumption: players are not moving if position has not changed for some number of frames.
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        Player* player = it->second;
        if (player->noMoveCounter == Config::getInt("Waddle_Animation_End_Delay"))
        {
            player->moving = false;
        }
        else
        {
            player->noMoveCounter++;
        }
    }

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
                Game::Vector3 scale = currMessage.object().scale();
                std::string modelPath = currMessage.object().modelpath();

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
                        case Game::PLAYER: 
                            
                            // Create new player
                            obj = new Player(id); 
                            players[id] = ((Player*) obj);

                            // Set client id based on model path
                            if (modelPath.compare("assets/models/characters/characterblue.obj") == 0)
                            {
                                ((Player*)obj)->setClientID(0);
                            }
                            else if (modelPath.compare("assets/models/characters/charactergreen.obj") == 0)
                            {
                                ((Player*)obj)->setClientID(1);
                            }
                            else if (modelPath.compare("assets/models/characters/characterpurple.obj") == 0)
                            {
                                ((Player*)obj)->setClientID(2);
                            }
                            else if (modelPath.compare("assets/models/characters/characterred.obj") == 0)
                            {
                                ((Player*)obj)->setClientID(3);
                            }
                            else
                            {
                                std::cerr << "Object Error: Only 4 players are supported." << std::endl;
                            }
                            break;

                        // Ingredient object.
                        case Game::INGREDIENT: 
                            obj = new Ingredient(id); 
                            ((Ingredient*)obj)->setQualityIndex(currMessage.object().quality());
                            break;

                        // Cookware object.
                        case Game::COOKWARE: obj = new Cookware(id); break;
                        
                        // Wall object.
                        case Game::WALL: obj = new Wall(id); break;

                        // Table object..
                        case Game::TABLE: obj = new Table(id); break;

                        // Plate object.
                        case Game::PLATE: obj = new Plate(id); break;

                        // Basic gameobject.
                        default: obj = new GameObject(id); break;
                    }
                    // Add object to window
                    window.addObject(id, obj);
                }

                // We got an updated model, set old model to null
                if (modelPath.compare(obj->getModelPath()) != 0)
                {
                    delete obj->model;
                    obj->model = NULL;
                }

                // Set new model path so we can reload
                obj->setModel(modelPath);

                // Set object parameters
                obj->setRotation(rotation);

                // The player is being scaled up which means the player picked up the vodka
                if (currMessage.object().has_scale() && currMessage.object().id() == this->objectId)
                {
                    glm::vec3 originalScale = obj->getScaleVec();
                    glm::vec3 newScale (scale.x(), scale.y(), scale.z());

                    // Bigger player
                    if (originalScale.x < newScale.x) {
                        // Play soviet national anthem
                        music.stop();
                        music.openFromFile("assets/audio/SovietAnthem.ogg");
                        music.setVolume(Config::getInt("Background_Music_Volume"));
                        music.setLoop(true);
                        music.play();

                        // Turn screen red
                        this->window.vodkaActive = true;
                    }

                    // Smaller player
                    else if (originalScale.x > newScale.x)
                    {
                        if (this->window.getRound() == DUNGEON_NUM) 
                        {
                            // Background music
                            music.stop();
                            music.openFromFile("assets/audio/8Bit_Paradise.ogg");
                            music.setVolume(Config::getInt("Background_Music_Volume"));
                            music.setLoop(true);
                            music.play();
                        }
                        // Revert screen color
                        this->window.vodkaActive = false; 
                    }
                }

                if (currMessage.object().has_scale()) obj->applyScale(glm::vec3(scale.x(), scale.y(), scale.z()));
                obj->setRender(render);
                obj->prevPosition = obj->getPosition();
                obj->setPosition(glm::vec3(location.x(), location.y(), location.z()));

                // Object type specific updates
                switch (obj->getObjectType())
                {
                    case PLAYER:
                        
                        // Is player cooking or not
                        ((Player*)obj)->cooking = currMessage.object().cooking();
                        
                        // Is player moving?
                        if (obj->getPosition() != ((Player*)obj)->prevPosition)
                        {
                            ((Player *) obj)->moving = true;
                            ((Player *) obj)->noMoveCounter = 0;
                        }
                        break;
                    default:
                        break;
                }

                // Removing cooking event message
                window.removeCookingEventMessage();
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
                auto currInventory = player->getInventory();

                // Get id of the object to be picked up, set the conditions
                Ingredient* pickup = (Ingredient*)window.objectsToRender[currMessage.inventory().id()];
                pickup->setName(currMessage.inventory().name());
                pickup->setQualityIndex(currMessage.inventory().qualityindex());
                pickup->setStatus(Ingredient::stringToIngredientStatus[currMessage.inventory().ingredientstatus()]);
                
                if(soundEffect.getStatus() == sf::SoundSource::Playing) {
                    soundEffect.stop();
                }

                if (soundBuffer.loadFromFile("assets/audio/Inventory_Pickup.wav")) {
                    soundEffect.setBuffer(soundBuffer);
                    soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                    soundEffect.play();
                }

                // Already exists
                if (currInventory->count(currMessage.inventory().id()) > 0)
                {
                    if (!currMessage.inventory().add()) 
                        player->removeFromInventory(pickup);
                }
                // Doesn't exist in inventory
                else
                {
                    player->addToInventory(pickup);
                }
                
                std::cout << "Picked up: " << pickup->getName() << std::endl;
                break;
            }

            case Game::ServerMessage::EventCase::kValidCook:
            {
                std::cout << "got valid cooking event from server" << std::endl;
                std::cout<<currMessage.validcook().message().at(0)<<std::endl;
                switch(currMessage.validcook().message().at(0)) {
                    case 'C': { 
                        soundBuffer.loadFromFile("assets/audio/Cutting.wav"); 
                        soundEffect.setBuffer(soundBuffer);
                        soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                        break;
                    }
                    case 'F': {
                        soundBuffer.loadFromFile("assets/audio/Frying.wav"); 
                        soundEffect.setBuffer(soundBuffer);
                        soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                        break;
                    }
                    case 'B': {
                        soundBuffer.loadFromFile("assets/audio/PotBoiling.wav"); 
                        soundEffect.setBuffer(soundBuffer);
                        soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                        break; 
                    }
                    case 'P': {
                        soundBuffer.loadFromFile("assets/audio/Dish.wav"); 
                        soundEffect.setBuffer(soundBuffer);
                        soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                        break; 
                    }
                    default: {
                        switch(rand()%6) {
                            case 0: soundBuffer.loadFromFile("assets/audio/nyet1.wav"); break;
                            case 1: soundBuffer.loadFromFile("assets/audio/nyet2.wav"); break;
                            case 2: soundBuffer.loadFromFile("assets/audio/nyet3.wav"); break;
                            case 3: soundBuffer.loadFromFile("assets/audio/nyet4.wav"); break;
                            case 4: soundBuffer.loadFromFile("assets/audio/nyet5.wav"); break;
                            case 5: soundBuffer.loadFromFile("assets/audio/nyet6.wav"); break;
                        }
                        soundEffect.setBuffer(soundBuffer);
                        soundEffect.setVolume(Config::getFloat("Sound_Effect_Volume"));
                    }
                }
                soundEffect.play();
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

            case Game::ServerMessage::EventCase::kInstruction:
            {
                int index = currMessage.instruction().index();
                std::string msg = currMessage.instruction().instructionmsg();
                if(currMessage.instruction().has_recipename() )
                    window.recipeName = currMessage.instruction().recipename();
                window.instructionStrings.insert(window.instructionStrings.begin()+index, msg);
                break;
            }

            case Game::ServerMessage::EventCase::kTime:
            {
                uint32_t seconds = currMessage.time().seconds();
                window.setTimer(seconds);
                break;
            }

            case Game::ServerMessage::EventCase::kRound:
            {
                window.updateRound(currMessage.round().type());         
                if (currMessage.round().type() == Game::RoundInfo_RoundState_KITCHEN_WAITING) {
                    music.stop();
                    music.openFromFile("assets/audio/Cook.ogg");
                    music.setVolume(Config::getInt("Background_Music_Volume"));
                    music.setLoop(true);
                    music.play();
                }
                std::cout << " received round update message " << currMessage.DebugString() << std::endl;
                break;
            }

            /// TODO: NEED TO CHANGE LATER
            case Game::ServerMessage::EventCase::kWin:
            {
                std::cout << " received win event " << std::endl;
                window.gameOver = true;

                // Win or lose
                if (currMessage.win().clientid() == clientId) {
                    window.gameWin = true;
                    music.stop();
                    music.openFromFile("assets/audio/SovietAnthem.ogg");
                    music.setLoop(true);
                    music.setVolume(Config::getInt("Background_Music_Volume"));
                    music.play();
                }
                else {
                    window.gameWin = false;
                    music.stop();
                    music.openFromFile("assets/audio/TheDustyAttic.ogg");
                    music.setLoop(true);
                    music.setVolume(Config::getInt("Background_Music_Volume"));
                    music.play();
                }

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
    Game::ClientMessage movementMessage;
    Game::ClientMessage rotationMessage;

    // Get key inputs and set direction of message
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_W) == GLFW_PRESS &&
    glfwGetKey(window.glfwViewport, GLFW_KEY_S) == GLFW_PRESS) {
        // Do nothing: The movements should cancel each other out.
    }
    else if (glfwGetKey(window.glfwViewport, GLFW_KEY_W) == GLFW_PRESS)
    {
        movementMessage.set_direction(Game::Direction::UP);  
    }
	else if (glfwGetKey(window.glfwViewport, GLFW_KEY_S) == GLFW_PRESS) 
    {
        movementMessage.set_direction(Game::Direction::DOWN); 
    }

    
    if (glfwGetKey(window.glfwViewport, GLFW_KEY_A) == GLFW_PRESS &&
    glfwGetKey(window.glfwViewport, GLFW_KEY_D) == GLFW_PRESS) {
        // Do nothing: The movements should cancel each other out.
    }
	else if (glfwGetKey(window.glfwViewport, GLFW_KEY_A) == GLFW_PRESS)
    {
        rotationMessage.set_direction(Game::Direction::LEFT); 
    }
	else if (glfwGetKey(window.glfwViewport, GLFW_KEY_D) == GLFW_PRESS)
    {
        rotationMessage.set_direction(Game::Direction::RIGHT); 
    }
    // Send message only if it has a direction associated with it
    if (movementMessage.has_direction()) {
        this->client.send(movementMessage);
    }
    if (rotationMessage.has_direction()) {
        this->client.send(rotationMessage);
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
}