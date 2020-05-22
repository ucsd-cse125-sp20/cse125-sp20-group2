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

/// TODO: Work in progress, will delete every repeat message
void GameProcessor::Preprocess(std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> & clientMsgs)
{
    for (auto & it : clientMsgs)
    {
        std::vector<Game::ClientMessage> & msgList = it.second;
        auto uniquePortion = std::unique(msgList.begin(), msgList.end(), google::protobuf::util::MessageDifferencer::Equals);
        msgList.erase(uniquePortion, msgList.end());
    }
}

void GameProcessor::Process(unsigned int clientId, Game::ClientMessage clientMsg, int tickCount)
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
                    std::cout << "Colliding with ingredients" << std::endl;
                    currIngredient->renderInvisible();
                    player->addToInventory(currIngredient);
                    player->addToScore(1);

                    // Creating message for collecting ingredient
                    Game::ServerMessage* newServerMsg = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true, currIngredient->getName());
                    specificMessages[player->getClientID()].push_back(newServerMsg);
                    newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                    messages.push_back(newServerMsg);

                    // Create message for score update
                    Game::ServerMessage* scoreUpdate = MessageBuilder::toScore(player->getScore());
                    this->specificMessages[player->getClientID()].push_back(scoreUpdate);

                    // Send updated map stuff
                     Game::ServerMessage* mapUpdate = MessageBuilder::toServerMessage(currIngredient);
                    this->messages.push_back(mapUpdate);
                }
            }
            else if (player->isColliding(currObject))
            {
                std::cout << "Detecting collision with an object" << std::endl;

                // Revert movement
                player->setPosition(originalPos);
                player->setRunSpeed(0);
                break;
            }
        }

        // Check for collisions for ingredients
        // const std::unordered_map<unsigned int, IngredientObject*>& ingredientObjects = this->state->getIngredientObjects();
        // for (auto currIngredientPair : ingredientObjects)
        // {
        //     IngredientObject* currIngredient = currIngredientPair.second;

        //     // Check for collision
        //     if (player->isColliding(currIngredient))
        //     {
        //         currIngredient->renderInvisible();
                
        //         // Update player state
        //         player->addToInventory(currIngredient);
        //         player->addToScore(1);

        //         Game::ServerMessage* scoreUpdate = MessageBuilder::toScore(player->getScore());
        //         this->specificMessages[clientId].push_back(scoreUpdate);

        //         Game::ServerMessage* moveToInventory = MessageBuilder::toInventoryServerMessage(currIngredient->getID(), true, currIngredient->getName());
        //         specificMessages[clientId].push_back(moveToInventory);
        //         Game::ServerMessage* mapUpdate = MessageBuilder::toServerMessage(currIngredient);
        //         messages.push_back(mapUpdate);
        //     }
        // }

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