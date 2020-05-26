#include <processors/EndProcessor.h>

void EndProcessor::initGameState(GameState* gameState, ServerGame* server)
{
    Game::ServerMessage* winMessage = MessageBuilder::toWinningMessage(gameState->getWinningPlayer()->getClientID());
    server->messages.push_back(winMessage);
}