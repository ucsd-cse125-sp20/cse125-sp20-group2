#include <game/GameProcessor.h>


GameProcessor::GameProcessor(GameState* gameState)
{
    this->state = gameState;
    
    /// TODO: Extend for different phases of the game
    Map* m = MapBuilder::getBasicMap();
    Recipe* r = RecipeBuilder::getBasicRecipe();
    MapBuilder::assignIngredientPositions(r, m);
    this->state->addMap(m);
    this->state->addRecipe(r);
    
}

GameProcessor::~GameProcessor()
{
    
}

void GameProcessor::process(unsigned int clientId, Game::ClientMessage clientMsg, int tickCount)
{
    switch (clientMsg.event_case())
    {
    // Modify gameState direction
    case Game::ClientMessage::EventCase::kDirection:
    {
        Player* player = this->state->getPlayerObject(clientId);
        glm::vec3 originalPos = player->getPosition();
        MovementProcessor::processMovement(player, clientMsg.direction(), tickCount);

        // See if colliding with any objects
        const std::unordered_map<unsigned int, GameObject*>& gameObjects = this->state->getObjects();
        for (auto currObjectPair : gameObjects)
        {
            GameObject* currObject = currObjectPair.second;

            // Check if the collision will be an inventory pickup event
            if (currObject->getObjectType() == INGREDIENT) {
                IngredientObject* currIngredient = (IngredientObject*) currObject;

                // Check for collision
                if (player->isColliding(currIngredient))
                {
                    currIngredient->renderInvisible();
                    player->addToInventory(currIngredient);
                    Game::ServerMessage* newServerMsg = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true, currIngredient->getName());
                    specificMessages[player->getClientID()].push_back(newServerMsg);
                    newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                    messages.push_back(newServerMsg);
                }
            }
            else if (player->isColliding(currObject))
            {
                std::cout << "Detecting a collision" << std::endl;
                // Revert movement
                player->setPosition(originalPos);
                player->setRunSpeed(0);
                break;
            }
        }

        // See if colliding with any players
        const std::unordered_map<unsigned int, Player*>& playerObjects = this->state->getPlayerObjects();
        for (auto currPlayerPair : playerObjects)
        {
            Player* currPlayer = currPlayerPair.second;
            if (player == currPlayer)
            {
                continue;
            }
            // Check for collisions
            if(player->isColliding(currPlayer))
            {
                // Revert the movement
                std::cout << "Detecting collision with another player" << std::endl;
                player->setPosition(originalPos);
                player->setRunSpeed(0);
                break;
            }
        }

        // Create serverMessage from vector
        Game::ServerMessage* newServerMsg = MessageBuilder::toServerMessage(player);
        messages.push_back(newServerMsg);

        // Reinstate movement into gamestate (may not need to do this)

        // gameObject->moveTo() bark bark
        break;
    }
    default:
        break;
    }

}