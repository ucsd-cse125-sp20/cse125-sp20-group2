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
        glm::vec3 initialPos = gameObject->getWorldPos();

        switch (clientMsg.direction())
        {
            case Game::Direction::UP:
            {
                initialPos.x += 1;
                
                break;
            }
            case Game::Direction::DOWN:
            {
                initialPos.x -= 1;
                break;
            }
            case Game::Direction::LEFT:
            {
                initialPos.z += 1;
                break;
            }
            case Game::Direction::RIGHT:
            {
                initialPos.z -= 1;
                break;
            }
            default:
                break;
        }

        gameObject->moveTo(initialPos);

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