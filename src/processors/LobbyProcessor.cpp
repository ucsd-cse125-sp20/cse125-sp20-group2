#include <processors/LobbyProcessor.h>

void LobbyProcessor::initLobby(GameState* gameState)
{
    gameState->setRoundTime(Config::getInt("Lobby_Round_Time"));
    // Map* m = MapBuilder::getBasicMap();
    // MapBuilder::assignIngredientPositions(r, m);
    // gameState->addWalls(m);
}

void LobbyProcessor::initDungeonWaiting(GameState* gameState) {
    // gameState->setRoundTime(Config::getInt("Dungeon_Waiting_Round_Time"));
}

void LobbyProcessor::Process(unsigned int clientId, Game::ClientMessage clientMsg, ServerGame* server)
{
    switch (clientMsg.event_case())
    {
        // The message is a movement event
        case Game::ClientMessage::EventCase::kDirection:
        {
            std::cout << "LobbyProcessor: Ignoring movement" << std::endl;
            break;
        }
        case Game::ClientMessage::EventCase::kReady:
        {
            std::cout << "LobbyProcessor: Received ready up" << std::endl;
            bool clientStatus = clientMsg.ready().ready();
            server->gameState.readyStatus[clientId] = clientStatus;
            break;
        }
        default:
        {
            std::cout << "LobbyProcessor: Undefined event" << std::endl;
            break;
        }
    }

}