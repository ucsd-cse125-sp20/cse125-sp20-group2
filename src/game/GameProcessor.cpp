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
    // The message is a movement event
    case Game::ClientMessage::EventCase::kDirection:
    {
        // Get the Player Object of the current client id
        Player* player = this->state->getPlayerObject(clientId);
        // 
        glm::vec3 originalPos = player->getPosition();
        MovementProcessor::processMovement(player, clientMsg.direction(), tickCount);

        // See if colliding with any objects
        const std::vector<GameObject*>& objects = this->state->getAllObjects();
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
                    specificMessages[player->getClientID()].push_back(newServerMsg);
                    newServerMsg = MessageBuilder::toServerMessage(currIngredient);
                    messages.push_back(newServerMsg);

                    // Create message for score update
                    Game::ServerMessage* scoreUpdate = MessageBuilder::toScore(player->getScore());
                    this->specificMessages[player->getClientID()].push_back(scoreUpdate);

                    // Send updated map stuff
                     Game::ServerMessage* mapUpdate = MessageBuilder::toServerMessage(currIngredient);
                    this->messages.push_back(mapUpdate);
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
        messages.push_back(newServerMsg);
        break;
    }
    default:
        break;
    }

}