#include <game/GameProcessor.h>


GameProcessor::GameProcessor(GameState* gameState)
{
    this->state = gameState;
    
    /// TODO: Extend for different phases of the game
    /*Map* m = MapBuilder::getBasicMap();
    this->state->addMap(m);*/
    ///FIXME: I don't know why, but this code causes a segfault!
    
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

            // Check for collision
            if (player->isColliding(currObject))
            {
                std::cout << "Detecting a collision" << std::endl;
                // Revert movement
                player->setPosition(originalPos);
                player->setRunSpeed(0);
                break;
            }
        }

        // Check for collisions for ingredients
        const std::unordered_map<unsigned int, IngredientObject*>& ingredientObjects = this->state->getIngredientObjects();
        for (auto currIngredientPair : ingredientObjects)
        {
            IngredientObject* currIngredient = currIngredientPair.second;

            // Check for collision
            if (player->isColliding(currIngredient))
            {
                currIngredient->renderInvisible();
                player->addToInventory(currIngredient);
                Game::ServerMessage* newServerMsg = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true);
                specificMessages[player->getID()].push_back(newServerMsg);
                newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                messages.push_back(newServerMsg);
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