#include <schema/Game.pb.h>
#include <objects/GameObject.h>

/**
 * This is a utility function to convert to ProtocolBuffer messages
 * */
class MessageBuilder
{
    /**
     * Converts a game object to a server message
     * */
    Game::ServerMessage toServerMessage(GameObject* object);
};