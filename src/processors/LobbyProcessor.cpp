#include <processors/LobbyProcessor.h>

void LobbyProcessor::initGameState(GameState* gameState)
{
    gameState->setRoundTime(15);
    // Map* m = MapBuilder::getBasicMap();
    // MapBuilder::assignIngredientPositions(r, m);
    // gameState->addMap(m);
}

void LobbyProcessor::Process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame* server)
{
    switch (clientMsg.event_case())
    {
        // The message is a movement event
        case Game::ClientMessage::EventCase::kDirection:
        {

        }
        default:
        {
            std::cout << "LobbyProcessor: Undefined event" << std::endl;
            break;
        }
    }

}