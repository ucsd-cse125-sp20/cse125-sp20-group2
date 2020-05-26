#include <processors/GameProcessor.h>

void GameProcessor::initGameState(GameState* gameState)
{
    gameState->setRoundTime(120);

    /// TODO: switch statement on dungeon or kitchen
    Map* m = MapBuilder::getBasicMap();
    Recipe* r = RecipeBuilder::getBasicRecipe();
    MapBuilder::assignIngredientPositions(r, m);
    gameState->addMap(m);
    gameState->addRecipe(r);

    // Move players to spawn locations
    for (auto playerPair : gameState->getPlayerObjects())
    {
        Player* currPlayer = playerPair.second;
        currPlayer->setPosition(m->spawningLocations.back());
        m->spawningLocations.pop_back();
    }
}

void GameProcessor::Process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame* server)
{
    switch (clientMsg.event_case())
    {
        // The message is a movement event
        case Game::ClientMessage::EventCase::kDirection:
        {
            // Get the Player Object of the current client id
            Player* player = server->gameState.getPlayerObject(clientId);
            // 
            glm::vec3 originalPos = player->getPosition();
            MovementProcessor::processMovement(player, clientMsg.direction(), server->getTick());

            // See if colliding with any objects
            const std::vector<GameObject*>& objects = server->gameState.getAllObjects();
            for (GameObject* currObject : objects)
            {
                // Don't collide with yourself kiddo
                if (currObject == player) continue;

                if (player->isColliding(currObject)) {
                    // Check if the collision will be an inventory pickup event
                    if (currObject->getObjectType() == INGREDIENT) {
                        std::cout << "Colliding with ingredients" << std::endl;
                        IngredientObject* currIngredient = (IngredientObject*) currObject;
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

                        // Create new ingredient
                        

                    } else {
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

void GameProcessor::createIngredient()
{

}