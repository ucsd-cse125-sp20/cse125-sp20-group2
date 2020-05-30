#include <processors/GameProcessor.h>

void GameProcessor::initDungeonPhase(GameState* gameState)
{
    /// TODO: switch statement on dungeon or kitchen
    DungeonMap* m = MapBuilder::getBasicDungeonMap();
    Recipe* r = RecipeBuilder::getBasicRecipe();
    MapBuilder::assignIngredientPositions(r, m);
    gameState->dungeonMap = m;
    gameState->addWalls(m);
    gameState->addRecipe(r);

    // Move players to spawn locations
    for (auto playerPair : gameState->getPlayerObjects())
    {
        Player* currPlayer = playerPair.second;
        currPlayer->setPosition(m->spawningLocations.back());
        m->spawningLocations.pop_back();
    }
    
    ///TODO: Spawn first ingredient
    spawnIngredient(gameState, r);
}

void GameProcessor::initKitchenPhase(GameState* gameState) {
    /// TODO: switch statement on dungeon or kitchen
    KitchenMap* m = MapBuilder::getBasicKitchenMap();
    gameState->addWalls(m);

    gameState->kitchenMap = m;

    for(auto it = m->cookwareObjects.begin(); it!= m->cookwareObjects.end(); it++) {
        gameState->cookwareObjects[(*it)->getID()] = *it;
    }

    for(auto it = m->tableList.begin(); it!= m->tableList.end(); it++) {
        gameState->worldObjects[(*it)->getID()] = *it;
    }
    
    // Move players to spawn locations
    for (auto playerPair : gameState->getPlayerObjects())
    {
        Player* currPlayer = playerPair.second;
        
        // Adding all instructions to players to keep track of the instructions' status in the future
        for( auto instruction : gameState->getRecipe()->instructionList ) {
            currPlayer->instructionSet.push_back(std::make_pair(instruction, false));
        }
        currPlayer->setPosition(m->spawningLocations.back());
        m->spawningLocations.pop_back();
    }
}

void GameProcessor::Process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame* server)
{
    switch (clientMsg.event_case())
    {
        case Game::ClientMessage::EventCase::kCookEvent:
        {
            std::cout << "got a cookevent" << std::endl;
            bool validCookEvent = false;
            // Retrive the ingredient
            Ingredient* ing = server->gameState.getIngredientObject(clientMsg.cookevent().objectid());
            std::unordered_map<unsigned int, Cookware*> & cookwarePairs = server->gameState.cookwareObjects;
            
            // Check for valid cookware in range
            for (auto & cookwarePair : cookwarePairs)
            {
                Player *p = server->gameState.getPlayerObject(clientId);
                glm::vec3 pPos = p->getPosition();
                Cookware *cookware = cookwarePair.second;

                glm::vec3 cPos = cookware->getPosition();
                float distance = sqrt(pow((pPos.x-cPos.x),2) + pow((pPos.y-cPos.y),2) + pow((pPos.z-cPos.z),2));
                
                

                // Check if within range and if cookware is busy
                std::cout << "this is the range" << distance << std::endl;

                if(distance <= Config::getFloat("Cooking_Event_Min_Distance") && !cookware->getBusy()) {
                    std::cout << "Cooking" << std::endl;    
                    
                    // Cookware is busy now
                    cookware->setBusy(true);
                    
                    int i = 0;
                    IngredientStatus origStatus = ing->getStatus();

                    if( cookware->cookIngredient(ing) ) {
                        IngredientStatus futureStatus = ing->getStatus();
                        // Set status back to original because cooking event just started
                        ing->setStatus(origStatus);
                        p->setFreeze(true);
                        CookEvent* event = new CookEvent();
                        event->player = p;
                        event->ingredient = ing;
                        event->cookware = cookware;
                        event->before = origStatus;
                        event->after = futureStatus;

                        event->terminationTime = std::chrono::high_resolution_clock::now()
                         + std::chrono::seconds(Config::getInt("Cook_Event_Time"));
                         
                        server->gameState.cookEvents.push_back(event);
                    }    
                }
                
                //if( boolCookEvent ) {
                    // Tell client that cooking event is valid and will last for 5 seconds
                    // If cookware is pan, set new model of pot with water (TODO: Saturday)
                    
                //} else {
                    // Terminate cooking event. Put an X over the player's head
                //}
                
            }

            break;
        }
        // The message is a movement event
        case Game::ClientMessage::EventCase::kDirection:
        {
            // Get the Player Object of the current client id
            Player* player = server->gameState.getPlayerObject(clientId);
            // 
            glm::vec3 originalPos = player->getPosition();

            // Ignore players if frozen
            if( player->getFreeze() )
                break;
                
            MovementProcessor::processMovement(player, clientMsg.direction(), server->getTick());

            // See if colliding with any objects
            const std::vector<GameObject*>& objects = server->gameState.getAllObjects();
            for (GameObject* currObject : objects)
            {
                // Don't collide with yourself kiddo
                if (currObject == player) continue;

                if (player->isColliding(currObject)) 
                {
                    // Check if the collision will be an inventory pickup event
                    if (currObject->getObjectType() == INGREDIENT) 
                    {
                        std::cout << "Colliding with ingredients" << std::endl;
                        Ingredient* currIngredient = (Ingredient*) currObject;
                        ///TODO: Add this back? 
                        currIngredient->renderInvisible();
                        player->addToInventory(currIngredient);
                        player->addToScore(1);

                        // Creating message for collecting ingredient
                        Game::ServerMessage* newServerMsg = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true, currIngredient->getName());
                        server->specificMessages[player->getClientID()].push_back(newServerMsg);
                        newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                        server->messages.push_back(newServerMsg);

                        // Create message for score update
                        Game::ServerMessage* scoreUpdate = MessageBuilder::toScore(player->getScore());
                        server->specificMessages[player->getClientID()].push_back(scoreUpdate);

                        // Send updated map stuff
                        Game::ServerMessage* mapUpdate = MessageBuilder::toServerMessage(currIngredient);
                        server->messages.push_back(mapUpdate);

                        ///TODO: Create new ingredient
                        Ingredient* newIngredient = spawnIngredient(&server->gameState, server->gameState.getRecipe());
                        Game::ServerMessage* ingredientUpdate = MessageBuilder::toServerMessage(newIngredient);
                        server->messages.push_back(ingredientUpdate);
                        //delete newIngredient;   
                    } 
                    else 
                    {
                        std::cout << "Detecting collision with an object" << std::endl;

                        // Revert movement
                        player->setPosition(originalPos);
                        player->setRunSpeed(0);
                        break;
                    }
                }
            }

            // Create serverMessage from vector
            Game::ServerMessage* newServerMsg = MessageBuilder::toServerMessage(player);
            server->messages.push_back(newServerMsg);
            break;
        }

        
        default:
        {
            std::cout << "GameProcessor: Undefined event" << std::endl;
            break;
        }
    }
}

Ingredient* GameProcessor::spawnIngredient(GameState* gameState, Recipe* recipe)
{
    std::cerr << "Spawning next ingredient..." << std::endl;
    
    // Make a copy of the ingredient
    Ingredient* ing = recipe->ingredientList.front();
    Ingredient* copy = RecipeBuilder::createIngredient(ing->getName());

    ///TODO: make position random
    copy->setPosition(ing->getPosition());

    // Add copy
    gameState->addIngredient(copy);

    // Push to the back of the list
    recipe->ingredientList.erase(recipe->ingredientList.begin());
    recipe->ingredientList.push_back(ing);

    // Return the copy of ingredient
    return copy;
}