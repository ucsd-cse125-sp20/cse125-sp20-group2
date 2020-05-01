#include <schema/Game.pb.h>
#include <objects/GameObject.h>

/**
 * This is a utility function to convert to ProtocolBuffer messages
 * */
class MessageBuilder
{
public:

    /**
     * Converts a game object to a server message
     * It is up to the caller to determine whether this is freed.
     * */
    static Game::ServerMessage* toServerMessage(GameObject* object)
    {
        std::cout << "wtf" << std::endl;

        if (object == NULL)
        {
            std::cout << "gameobject is null :(" << std::endl;
        }
        // TODO: Check if sending server message deallocates Vector
        Game::Vector3* vector = toVector(object->getWorldPos());

        Game::Object* msgObj = new Game::Object();
        msgObj->set_id(object->getID());
        msgObj->set_rotation(object->getRotation());
        // TODO: Assume players only for now
        msgObj->set_type(Game::ObjectType::PLAYER); 
        msgObj->set_allocated_worldposition(vector);

        Game::ServerMessage* message = new Game::ServerMessage();
        message->set_allocated_object(msgObj);
        return message;
    }

    /**
     * Returns a Vector3 protocol message from a glm vec3
     * It is up to the caller to determine whether this is freed.
     * */
    static Game::Vector3* toVector(glm::vec3 position)
    {
        std::cout << "qqq" << std::endl;
        Game::Vector3* vector = new Game::Vector3();
        vector->set_x(position.x);
        vector->set_y(position.y);
        vector->set_z(position.z);
        std::cout << "dummbbb" << std::endl;

        return vector;
    }
};