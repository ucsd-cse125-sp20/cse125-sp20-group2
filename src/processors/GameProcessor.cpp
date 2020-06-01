#include <processors/GameProcessor.h>

void GameProcessor::initLobbyPhase(GameState *gameState)
{
    gameState->setRoundTime(Config::getInt("Lobby_Round_Time"));
    // Map* m = MapBuilder::getBasicMap();
    // MapBuilder::assignIngredientPositions(r, m);
    // gameState->addWalls(m);
}

void GameProcessor::initDungeonWaiting(GameState* gameState)
{
    gameState->setRoundTime(Config::getInt("Dungeon_Waiting_Round_Time"));
}

void GameProcessor::initDungeonPhase(GameState *gameState, ServerGame *server)
{
    // Create the map
    DungeonMap *m = MapBuilder::getBasicDungeonMap();
    Recipe *r = RecipeBuilder::getBasicRecipe();
    MapBuilder::assignIngredientPositions(r, m);
    gameState->dungeonMap = m;
    gameState->addWalls(m);
    gameState->addRecipe(r);

    // Give all connected players basic ingredients
    // Make sure all those ingredients are in gameState
    for (auto & playerPair : gameState->getPlayerObjects())
    {
        unsigned int clientId = playerPair.first;
        Player *currPlayer = playerPair.second;
        std::cout << "this is the ingredient list size " << r->ingredientList.size() << std::endl;
        for (auto ingredientCopy : r->ingredientList)
        {
            // Set low quality, add to player, add to gamestate, make invisible
            Ingredient *currIngredient = RecipeBuilder::createIngredient(ingredientCopy->getName());
            currIngredient->setQualityIndex(BAD_QUALITY);
            currIngredient->renderInvisible();

            currPlayer->addToInventory(currIngredient);
            gameState->addIngredient(currIngredient);

            // Create message for collecting ingredient
            Game::ServerMessage *serverMsg =
                MessageBuilder::toInventoryServerMessage(currIngredient->getID(),
                                                         true,
                                                         currIngredient->getName(),
                                                         currIngredient->getStatus(),
                                                         currIngredient->getQualityIndex());

            // put onto pending to send out
            server->specificMessages[clientId].push_back(serverMsg);
        }
    }

    ///TODO: Spawn first ingredient
    spawnIngredient(gameState, r);
}

void GameProcessor::initPlayersLocations(Map *map, GameState *gameState)
{
    // Move players to spawn locations
    for (auto playerPair : gameState->getPlayerObjects())
    {
        Player *currPlayer = playerPair.second;
        currPlayer->setPosition(map->spawningLocations.back());
        map->spawningLocations.pop_back();
    }
}

void GameProcessor::initKitchenPhase(GameState *gameState)
{
    KitchenMap *m = MapBuilder::getBasicKitchenMap(gameState);

    gameState->addWalls(m);

    gameState->kitchenMap = m;

    for (auto it = m->cookwareObjects.begin(); it != m->cookwareObjects.end(); it++)
    {
        gameState->cookwareObjects[(*it)->getID()] = *it;
    }

    for (auto it = m->tableList.begin(); it != m->tableList.end(); it++)
    {
        gameState->worldObjects[(*it)->getID()] = *it;
    }

    for (auto it = m->plateObjects.begin(); it != m->plateObjects.end(); it++)
    {
        gameState->plateObjects[(*it)->getID()] = *it;
    }

    // Adding all instructions to players to keep track of the instructions' status in the future
    for (auto playerPair : gameState->getPlayerObjects())
    {
        Player *currPlayer = playerPair.second;
        for (auto instruction : gameState->getRecipe()->instructionList)
        {
            currPlayer->instructionSet.push_back(std::make_pair(instruction, false));
        }
    }
}

void GameProcessor::processLobby(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame *server)
{
    switch (clientMsg.event_case())
    {
        case Game::ClientMessage::EventCase::kReady:
        {
            std::cout << "received ready up event in processing lobby" << std::endl;
            bool clientStatus = clientMsg.ready().ready();
            server->gameState.readyStatus[clientId] = clientStatus;
            break;
        }
        default:
        {
            std::cout << "ignored by lobby processing" << std::endl;
            break;
        }
    }
}

void GameProcessor::process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame *server)
{
    switch (clientMsg.event_case())
    {
    case Game::ClientMessage::EventCase::kCookEvent:
    {
        std::cout << "got a cookevent" << std::endl;
        // Retrive the ingredient
        Ingredient *ing = server->gameState.getIngredientObject(clientMsg.cookevent().objectid());
        std::unordered_map<unsigned int, Cookware *> &cookwarePairs = server->gameState.cookwareObjects;
        bool validCookEvent;
        std::string msg = "Invalid";

        Player *p = server->gameState.getPlayerObject(clientId);
        glm::vec3 pPos = p->getPosition();

        // Check for valid cookware in range
        for (auto &cookwarePair : cookwarePairs)
        {
            Cookware *cookware = cookwarePair.second;
            glm::vec3 cPos = cookware->getPosition();
            float distance = sqrt(pow((pPos.x - cPos.x), 2) + pow((pPos.y - cPos.y), 2) + pow((pPos.z - cPos.z), 2));

            // Check if within range and if cookware is busy
            std::cout << "this is the range" << distance << std::endl;
            IngredientStatus origStatus = ing->getStatus();

            if (distance <= Config::getFloat("Cooking_Event_Min_Distance") && !cookware->getBusy() && cookware->cookIngredient(ing))
            {
                std::cout << "Cooking" << std::endl;

                validCookEvent = true;
                // Cookware is busy now
                cookware->setBusy(true);

                IngredientStatus futureStatus = ing->getStatus();
                // Set status back to original because cooking event just started
                ing->setStatus(origStatus);
                p->setFreeze(true);
                CookEvent *event = new CookEvent();
                event->player = p;
                event->ingredient = ing;
                event->cookware = cookware;
                event->before = origStatus;
                event->after = futureStatus;

                event->terminationTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(Config::getInt("Cook_Event_Time"));

                server->gameState.cookEvents.push_back(event);

                validCookEvent = true;

                if (cookware->getName() == PAN)
                {
                    msg = "Frying the ";
                }
                else if (cookware->getName() == POT)
                {
                    msg = "Boiling the ";
                }
                else if (cookware->getName() == CUTTING_BOARD)
                {
                    msg = "Cutting the ";
                }
                msg = msg + ing->getName();
            }
        }

        /// TODO: may not be in the right spot, but see if close to any plates, and interact with them
        for (auto &platePair : server->gameState.plateObjects)
        {
            Plate *plate = platePair.second;
            glm::vec3 cPos = plate->getPosition();

            float distance = sqrt(pow((pPos.x - cPos.x), 2) + pow((pPos.y - cPos.y), 2) + pow((pPos.z - cPos.z), 2));
            if (distance < Config::getFloat("Plate_Event_Min_Distance"))
            {
                std::cout << "you are interacting with a plate" << std::endl;
            }
        }

        Game::ServerMessage *newServerMsg = MessageBuilder::toValidCookingEvent(msg, validCookEvent);
        server->specificMessages[clientId].push_back(newServerMsg);

        break;
    }
    // The message is a movement event
    case Game::ClientMessage::EventCase::kDirection:
    {
        // Get the Player Object of the current client id
        Player *player = server->gameState.getPlayerObject(clientId);
        //
        glm::vec3 originalPos = player->getPosition();

        // Cancel cooking event if frozen
        if (player->getFreeze())
        {
            std::vector<CookEvent *>::iterator iter;
            for (iter = server->gameState.cookEvents.begin(); iter != server->gameState.cookEvents.end();)
            {
                CookEvent *cookEvent = *iter;
                if (cookEvent->player == player)
                {
                    std::cout << "found a player and their cookevent" << std::endl;
                    player->setFreeze(false);
                    cookEvent->cookware->setBusy(false);
                    iter = server->gameState.cookEvents.erase(iter);
                    break;
                }
                else
                    iter++;
            }
        }

        MovementProcessor::processMovement(player, clientMsg.direction(), server->getTick());

        // See if colliding with any objects
        const std::vector<GameObject *> &objects = server->gameState.getAllObjects();
        for (GameObject *currObject : objects)
        {
            // Don't collide with yourself kiddo
            if (currObject == player)
                continue;

            if (player->isColliding(currObject))
            {
                // Check if the collision will be an inventory pickup event
                if (currObject->getObjectType() == INGREDIENT)
                {
                    std::cout << "Colliding with ingredients" << std::endl;
                    Ingredient *currIngredient = (Ingredient *)currObject;
                    ///TODO: Add this back?
                    currIngredient->renderInvisible();
                    player->addToInventory(currIngredient);
                    player->addToScore(1);

                    // Creating message for collecting ingredient
                    Game::ServerMessage *newServerMsg = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true, currIngredient->getName(),
                                                                                                 currIngredient->getStatus(), currIngredient->getQualityIndex());
                    server->specificMessages[player->getClientID()].push_back(newServerMsg);
                    newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                    server->messages.push_back(newServerMsg);

                    // Create message for score update
                    Game::ServerMessage *scoreUpdate = MessageBuilder::toScore(player->getScore());
                    server->specificMessages[player->getClientID()].push_back(scoreUpdate);

                    // Send updated map stuff
                    Game::ServerMessage *mapUpdate = MessageBuilder::toServerMessage(currIngredient);
                    server->messages.push_back(mapUpdate);

                    ///TODO: Create new ingredient
                    Ingredient *newIngredient = spawnIngredient(&server->gameState, server->gameState.getRecipe());
                    Game::ServerMessage *ingredientUpdate = MessageBuilder::toServerMessage(newIngredient);
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
        Game::ServerMessage *newServerMsg = MessageBuilder::toServerMessage(player);
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

Ingredient *GameProcessor::spawnIngredient(GameState *gameState, Recipe *recipe)
{
    std::cerr << "Spawning next ingredient..." << std::endl;

    // Make a copy of the ingredient
    Ingredient *ing = recipe->ingredientList.front();
    Ingredient *copy = RecipeBuilder::createIngredient(ing->getName());

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

void GameProcessor::initEndPhase(GameState *gameState, ServerGame *server)
{
    Game::ServerMessage *winMessage = MessageBuilder::toWinningMessage(gameState->getWinningPlayer()->getClientID());
    server->messages.push_back(winMessage);
}