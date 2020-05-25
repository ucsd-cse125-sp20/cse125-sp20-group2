#pragma once 

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
        if (object == NULL)
        {
            std::cout << "gameobject is null :(" << std::endl;
            exit(1);
        }

        /// TODO: Check if sending server message deallocates Vector
        Game::Vector3* vector = toVector(object->getPosition());

        // Set position/location/id (universal aspects of all game objects)
        Game::Object* msgObj = new Game::Object();
        msgObj->set_allocated_worldposition(vector);
        msgObj->set_rotation(object->getRotation());
        msgObj->set_id(object->getID());
        msgObj->set_allocated_modelpath(new std::string(object->getModelPath()));
        msgObj->set_render(object->getRender());
        
        // Set type
        switch (object->getObjectType()) {
            case 0:
                msgObj->set_type(Game::ObjectType::OBJECT); break;
            case 1:
                msgObj->set_type(Game::ObjectType::PLAYER); break;
            case 2:
                msgObj->set_type(Game::ObjectType::INGREDIENT); break;
            case 3:
                msgObj->set_type(Game::ObjectType::COOKWARE); break;
            case 4:
                msgObj->set_type(Game::ObjectType::WALL); break;
        }

        // Put allocated object into message to be sent
        Game::ServerMessage* message = new Game::ServerMessage();
        message->set_allocated_object(msgObj);
        return message;
    }

    /**
     * Convert inventory id to message for inventory pickup event
     * */
    static Game::ServerMessage* toInventoryServerMessage(int id, bool add, std::string name) {
        Game::Inventory* msgObj = new Game::Inventory();
        msgObj->set_id(id);
        msgObj->set_add(add);
        msgObj->set_name(name);
        Game::ServerMessage* message = new Game::ServerMessage();
        message->set_allocated_inventory(msgObj);
        return message;
    }

    /**
     * Creates client initialization message
     * */
    static Game::ServerMessage* toClientInfo(uint32_t clientId, uint32_t objectId)
    {
        Game::ServerMessage* serverMessage = new Game::ServerMessage();
        Game::ClientInfo* clientInfo = new Game::ClientInfo();
        clientInfo->set_clientid(clientId);
        clientInfo->set_objectid(objectId);
        serverMessage->set_allocated_clientinfo(clientInfo);
        return serverMessage;
    }

    /**
     * Returns a Vector3 protocol message from a glm vec3
     * It is up to the caller to determine whether this is freed.
     * */
    static Game::Vector3* toVector(glm::vec3 position)
    {
        Game::Vector3* vector = new Game::Vector3();
        vector->set_x(position.x);
        vector->set_y(position.y);
        vector->set_z(position.z);

        return vector;
    }

    /**
     * Creates a ServerMessage from a score
     * */
    static Game::ServerMessage* toScore(int value)
    {
        Game::Score* score = new Game::Score();
        score->set_currscore(value);

        Game::ServerMessage* message = new Game::ServerMessage();
        message->set_allocated_score(score);
        return message;
    }

    /**
     * Creates a ServerMessage to update the round state
     * */ 
    static Game::ServerMessage* toRoundUpdate(Game::RoundInfo::RoundState currState)
    {
        Game::RoundInfo* roundInfo = new Game::RoundInfo();
        roundInfo->set_type(currState);

        Game::ServerMessage* message = new Game::ServerMessage();
        message->set_allocated_round(roundInfo);
        return message;
    }

    /*
    * Creates a client ready / unready message
    * */
    static Game::ClientMessage* toReadyMessage(bool isReady)
    {
        Game::ReadyState* readyMsg = new Game::ReadyState();
        readyMsg->set_ready(isReady);

        Game::ClientMessage* clientMsg = new Game::ClientMessage();
        clientMsg->set_allocated_ready(readyMsg);
        return clientMsg;
    }
};