#include <game/GameProcessor.h>
#include <schema/Game.pb.h>
#include <util/MessageBuilder.h>

GameProcessor::GameProcessor(GameState* gameState)
{
    this->state = gameState;
}

GameProcessor::~GameProcessor()
{
    
}

void GameProcessor::process(unsigned int clientId, Game::ClientMessage clientMsg)
{
    switch (clientMsg.event_case())
    {
    // Modify gameState direction
    case Game::ClientMessage::EventCase::kDirection:
    {
        GameObject* gameObject = this->state->getPlayerObject(clientId);
        glm::vec3 originalPos = gameObject->getPosition();
        glm::vec3 newPos = gameObject->getPosition();

        switch (clientMsg.direction())
        {
            case Game::Direction::UP:
            {
                newPos.x += 0.01;
                break;
            }
            case Game::Direction::DOWN:
            {
                newPos.x -= 0.01;
                break;
            }
            case Game::Direction::LEFT:
            {
                newPos.z += 0.01;
                break;
            }
            case Game::Direction::RIGHT:
            {
                newPos.z -= 0.01;
                break;
            }
            default:
                break;
        }

        // Temporarily set the new position
        gameObject->setPosition(newPos);

        // Get all objects, see if colliding with any
        const std::unordered_map<unsigned int, GameObject*>& gameObjects = this->state->getObjects();
        for (auto currObjectPair : gameObjects)
        {
            GameObject* currObject = currObjectPair.second;
            if (gameObject == currObject)
            {
                continue;
            }
            // Check for collision
            if (gameObject->isColliding(currObject))
            {
                std::cout << "Detecting a collision"<< std::endl;
                // Revert movement
                gameObject->setPosition(originalPos);
                break;
            }
        }

        // Create serverMessage from vector
        Game::ServerMessage* newServerMsg = MessageBuilder::toServerMessage(gameObject);
        messages.push_back(newServerMsg);

        // Do cool math stuff to movement (LATER)
        // Reinstate movement into gamestate (may not need to do this)

        // gameObject->moveTo() bark bark
        break;
    }
    default:
        break;
    }

}